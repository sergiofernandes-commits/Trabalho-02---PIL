# Projeto Control PIL - Conversor buck (TMS320F28379D)

Este repositório contém uma implementação prática de **Processor-In-the-Loop (PIL)** para a validação e controle de um conversor estático Buck utilizando o microcontrolador C2000 TMS320F28379D. O ecossistema integra simulações numéricas no **PLECS**, projeto de malhas de controle em **Python**, e a execução do algoritmo em tempo real no hardware via comunicação serial (SCI).


## Estrutura do Projeto

O repositório está organizado da seguinte forma:
* **`main.c`**: Código principal da CPU1, responsável pela inicialização dos periféricos (SysCtl, GPIO, etc.), configuração do clock do sistema e gerenciamento do loop de controle em tempo real.
* **`src/scicomm.c` e `src/scicomm.h`**: Módulos de baixo nível desenvolvidos com DriverLib para gerenciar a interface de comunicação serial (SCI/UART). Realizam o empacotamento, transmissão e recepção dos dados de telemetria entre o DSP e a simulação PIL.
* **`plecs_sim/`**: Contém os arquivos de simulação do conversor buck:
  * `vmc_buck_continuous.plecs`: Simulação em tempo contínuo do conversor buck em malha fechada.
  * `vmc_buck.plecs`: Simulação SIL do conversor buck em malha fechada.
  * `vmc_buck_pil.plecs`: Bloco de simulação estruturado para rodar no modo Processor-In-the-Loop, onde o conversor roda no PLECS e o controlador roda no microcontrolador.
  * `vmc_buck_model_validation.plecs`:Validação do modelo do conversor buck.
* **`python/projeto_controlador.py`**: Script em Python utilizado para o cálculo analítico dos ganhos do controlador digital (VMC - *Voltage Mode Control*) e sintonia das malhas com base nos parâmetros dinâmicos do conversor.


## Arquitetura do Sistema (PIL)

A metodologia PIL permite validar o código de controle no próprio hardware do microcontrolador antes da aplicação em uma bancada de potência física:
1. O software **PLECS** calcula o estado das tensões e correntes do circuito do conversor buck a cada passo de simulação.
2. Os dados de medição (ex: tensão de saída $V_{out}$) são empacotados e transmitidos via porta serial para o TMS320F28379D através das rotinas do `scicomm.c`.
3. O microcontrolador recebe os dados por interrupção da SCI, executa o algoritmo de controle digital projetado no script Python, e calcula a nova razão cíclica ($D$).
4. O valor de controle calculado é enviado de volta ao PLECS via serial, que o aplica ao interruptor estático (MOSFET) no próximo passo de integração.


## Como Executar o Projeto

1. Abra o Code Composer Studio (CCS) e importe o projeto contido na raiz deste repositório.
2. Compile o código utilizando o perfil **Debug (RAM)** e carregue o binário `.out` no TMS320F28379D através do arquivo de configuração de alvo `targetConfigs/TMS320F28379D.ccxml`.
3. Coloque o microcontrolador em modo de execução (*Resume/Run*).
4. Abra o arquivo `plecs_sim/vmc_buck_pil.plecs` no PLECS.
5. Certifique-se de que a porta COM configurada no bloco PIL do PLECS coincide com a porta virtual associada ao LaunchPad no seu sistema operacional.
6. Inicie a simulação no PLECS para visualizar as curvas de resposta dinâmica do conversor Buck controladas em tempo real pelo chip.
