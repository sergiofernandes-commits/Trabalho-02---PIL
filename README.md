# Comunicação Serial do Plecs com micontrolador C2000 para execução do Controlador Proporcional Ressonante de um inversor monofásico simulado no PLECS - PIL 

## Descrição

O objetivo é simular um inversor monofásico no Plecs e realizar seu controle no micontrolador PLECS. O controle utilizado é de um controlador proporcional ressonante.  


---

# Funcionamento Geral

O sistema opera continuamente realizando as seguintes etapas:

1. O conversor monofásico é simulado no software PLECS que gera a corrente injetada na rede eletrica;
2. o Micontrolador recebe a corrente gerada do conversor simulado no PLECS via porta serial;
3. O micontrolador envia o sinal da CPU para o CLA;
4. O CLA processa o algoritmo de controle;
5. Ao finalizar o processamento, a interrupção do CLA é acionada;
6. A CPU envia o resultado do controlador de volta pela interface SCI;
7. O PLECS realiza o chaveamento do sinal recebido do micontrolador.


---

# Estrutura do Código

## main()

A função principal é responsável por:

* inicializar o dispositivo;
* configurar os periféricos;
* inicializar as interrupções;
* habilitar as interrupções globais;
* monitorar continuamente a recepção de dados pela SCI.

Quando um valor do tipo float é recebido pela serial, ele é armazenado na variável compartilhada `vo`, e a execução do CLA é imediatamente iniciada.

---

## cla1Isr1()

Esta interrupção é executada automaticamente após o término da tarefa do CLA.

Suas funções são:

* transmitir pela SCI o resultado calculado pelo controlador (`cla_output`);
* limpar a interrupção do CLA para permitir novas execuções.

---

# Comunicação CPU × CLA

As variáveis compartilhadas entre CPU e CLA são armazenadas em regiões específicas de memória utilizando a diretiva `#pragma DATA_SECTION`.

## Entrada

* `vo` → sinal recebido pela comunicação serial.

## Saída

* `cla_output` → saída calculada pelo controlador.

## Estados internos

Também são compartilhadas as variáveis de estado do controlador digital:

* `x0`
* `x1`
* `x2`
* `y0`
* `y1`
* `y2`

Essas variáveis representam os estados internos do algoritmo implementado no CLA, permitindo a execução de controladores discretos baseados em equações de diferenças.

---

# Fluxo de Execução

```text
Computador
     │
     ▼
 Comunicação SCI
     │
     ▼
CPU (C2000)
     │
Recebe "vo"
     │
     ▼
Dispara CLA
     │
     ▼
Controlador Digital
     │
     ▼
cla_output
     │
     ▼
CPU
     │
     ▼
SCI
     │
     ▼
Computador
```

---

# Comunicação Serial

A troca de dados é realizada pelas funções:

* `protocolReceiveData()` → recepção de dados pela SCI;
* `protocolSendData()` → transmissão dos resultados pela SCI.

Neste projeto são transmitidos valores do tipo **float (32 bits)**.

---

# Memória Compartilhada

A comunicação entre CPU e CLA utiliza as regiões:

* **CpuToCla1MsgRAM** → envio de dados da CPU para o CLA;
* **Cla1ToCpuMsgRAM** → retorno dos resultados do CLA para a CPU.

# Simulação no Plecs 

- O programa da simulação no PLECS está localizado na pasta plecs_sim → Trabalho 02.plecs



