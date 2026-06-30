# Comunicação Serial com CLA para Execução de Controlador Digital

## Descrição

Este projeto implementa a comunicação entre um computador e um microcontrolador **Texas Instruments C2000** por meio da interface **SCI (Serial Communication Interface)**. O objetivo é executar um algoritmo de controle no **Control Law Accelerator (CLA)**, recebendo uma amostra pela porta serial, processando-a no CLA e retornando o resultado ao computador.

Essa arquitetura é utilizada em aplicações **Processor-in-the-Loop (PIL)**, permitindo validar algoritmos embarcados executando o controlador diretamente no hardware.

---

# Funcionamento Geral

O sistema opera continuamente realizando as seguintes etapas:

1. Recebe uma amostra de entrada pela interface SCI;
2. Armazena o valor recebido na memória compartilhada entre CPU e CLA;
3. Dispara a execução da tarefa do CLA;
4. O CLA processa o algoritmo de controle;
5. Ao finalizar o processamento, a interrupção do CLA é acionada;
6. A CPU envia o resultado do controlador de volta pela interface SCI.

Todo o processamento ocorre em tempo real.

---

# Estrutura do Código

## main()

A função principal é responsável por:

* inicializar o dispositivo;
* configurar os periféricos;
* inicializar as interrupções;
* habilitar as interrupções globais;
* monitorar continuamente a recepção de dados pela SCI.

Quando um valor do tipo **float** é recebido pela serial, ele é armazenado na variável compartilhada `vo`, e a execução do CLA é imediatamente iniciada.

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

Esse mecanismo evita cópias adicionais de memória e reduz o tempo de comunicação entre os dois processadores.

---


# Objetivo

O objetivo deste projeto é validar a execução de algoritmos de controle diretamente no **Control Law Accelerator (CLA)**, utilizando a interface serial como meio de comunicação com um computador. Essa abordagem permite verificar o comportamento do controlador em hardware real, reduzindo a diferença entre a simulação e a implementação embarcada e servindo como etapa intermediária antes da validação em Hardware-in-the-Loop (HIL).
