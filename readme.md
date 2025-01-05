<section>   
    <h1>📢 SO: Simulador da Arquitetura de Von Neumann e Pipeline MIPS</h1>   
    <p>
        Criado em Dezembro de 2024, o projeto do<strong> Simulador da Arquitetura de Von Neumann e Pipeline MIPS</strong> foi desenvolvido em virtude de atender as demandas do curso de Sistemas Operacionais, ministrado pelo professor <a href="https://www.linkedin.com/in/michelpiressilva/?originalSubdomain=br" target="_blank">Michel Pires Da Silva</a> no Centro Federal de Educação Tecnológica de Minas Gerais (<a href="https://www.divinopolis.cefetmg.br/" target="_blank">CEFET-MG</a>).
    </p>
    <p>
        <a href="https://github.com/LuanLuL/Sistemas_Operacionais/tree/master/documentation" target="_blank">Acesse os enunciados [AQUI]</a>
    </p>
    <div>
          <p>
            Desenvolvido apenas por mim, <a href="https://www.linkedin.com/in/luan-santos-9bb01920b/" target="_blank">Luan Gonçalves Santos</a>, o projeto consiste em um simulador com arquitetura multicore e suporte a preempção que processa instruções MIPS através de pseudoparalelismo realizado por threads controladas pelo mutex; Sendo este desenvolvido na linguagem C++.
         </p>   
    </div>
</section>

<section>
    <h2>📌Estrutura Geral</h2>
    <p>
        O simulador é composto por classes que representam a <b>1) CPU</b>, a qual possui uma <b>2) Unidade de Controle</b>, a <b>3) Memória RAM</b>, um <b>4) Banco de Registradores</b> e a <b>5) Tabela de Recursos</b>. Além disso, o código principal executa as instruções a partir de arquivos TXTs que são abstraídos como sendo a <b>6) Memória ROM</b> da arquitetura. Com o intuito de organizar e proporcionar uma visão simplificada da comunicação entre essas estruturas do simulador, foi-se criado um desenho das classes e structs utilizadas no sistema, observe mais na Figura 1.  
    </p>
    <br>
    <p align="center">
        <img src="img/Esquema_SO.png">
        <br><br><strong>Figura 1:</strong> Desenho sobre o Simulador da Arquitetura de Von Neumann e Pipeline MIPS
    </p>
    <br>
    <p>
        O simulador segue um fluxo básico de execução de instruções, semelhante ao ciclo de instruções de um processador com pipeline, com as fases de busca (fetch), decodificação (decode), execução (execute), acesso à memória (memory access), e escrita de volta (write back).
    </p>
    <div>
        <h3>1) CPU</h3>
        <p>
            A classe <code>CentralProcessesUnit</code> é responsável pelo controle do fluxo de instruções. Isso acontece a partir das seguintes funções:
        </p>
        <ul>
            <li><code>CentralProcessesUnit::execute()</code>: Implementa a lógica de um escalonador com <i>time-sharing</i>, utilizando <i>time quantum</i> para dividir o tempo da CPU entre diferentes processos. A função carrega uma página de memória do processo na RAM (swapProgram), executa instruções enquanto o contador de programa (PC) estiver dentro do tamanho do processo e não exceder o tempo máximo (TIME_QUANTUM). Durante a execução, a unidade lógica aritmética (ULA) processa as instruções, os ciclos de clock são incrementados, e, se o limite de time quantum for atingido, o processo é suspenso e outro pode ser escalonado. Quando o processo termina, ou é preemptado, a página de memória é descarregada da RAM.</li>
            <li><code>CentralProcessesUnit::swapProgram()</code>: Esta função gerencia a troca de contextos entre processos em execução e a memória principal (RAM), essencial para o funcionamento de um simulador baseado na arquitetura de Von Neumann e no pipeline MIPS. Dependendo do tipo de troca especificado pela variável <i>typeOfSwap</i>, a CPU executa diferentes ações para lidar com o estado dos registradores e da memória.
                <ol type="I">
                    <li>
                        <strong>Tipo 0 - Processo finalizado:</strong> Neste caso, a função é responsável por limpar todos os recursos associados a um processo que terminou sua execução. Os registradores utilizados pelo processo são marcados como limpos, indicando que estão disponíveis para novos processos. Além disso, todas as células de memória na RAM associadas ao processo (identificadas pelo idProcess) são apagadas. Esse procedimento garante que nenhum dado residual permaneça na CPU ou na memória, evitando conflitos futuros. Por fim, a CPU é liberada, ficando pronta para receber outro processo.
                    </li>
                    <li>
                        <strong>Tipo 1 - Time Quantum expirado:</strong> Quando o tempo de time quantum de um processo termina, esta lógica salva o estado atual do processo para que ele possa ser retomado posteriormente. O valor do contador de programa (PC) é armazenado em uma <i>struct</i> do processo (MemoryPage) para registrar o ponto onde foi interrompido. Os valores dos registradores usados pelo processo são copiados para a memória RAM, preservando o estado atual do processo, por meio da <i>struct MemoryCell</i>, que representa uma célula de memória. Após o salvamento, os registradores são marcados como limpos e a CPU é liberada, permitindo que outro processo seja escalonado. Essa abordagem implementa o compartilhamento de recursos relacioando a memória principal (RAM), fundamental em sistemas multitarefa.
                    </li>
                    <li>
                        <strong>Tipo 2 - Novo processo entrando:</strong> Quando um novo processo é escalonado para execução, esta lógica inicializa o contexto necessário na CPU. O status da CPU é atualizado para ocupado, indicando que está em uso. O contador de programa (PC) é configurado para retomar a execução do processo no ponto onde ele foi interrompido anteriormente. Em seguida, os valores da memória RAM associados ao processo (registradores e valores) são carregados nos registradores da CPU, restaurando o estado do processo para continuar sua execução. Isso assegura a continuidade sem perda de dados ou inconsistências no processamento.
                    </li>
                </ol>
            Essa lógica de troca de contexto é essencial para gerenciar múltiplos processos em ambientes multitarefa, garantindo que os dados de cada processo sejam preservados e restaurados corretamente. Em caso de tipo de troca inválido, uma exceção é lançada para evitar comportamento inesperado.
            </li>
        </ul>
    </div>
    <div>
        <h3> 2) Unidade de Controle </h3>
        <p>
            A classe <code>ControlUnit</code> representa a Unidade de Controle de um simulador de arquitetura de computadores, sendo responsável por gerenciar a execução de instruções de um processo em nível de registradores e memória. Atributos privados, como <code>bankOfRegistrars</code>, armazenam um banco de registradores que é essencial para o controle e execução das instruções, enquanto o método <code>ControlUnit::splitLineOfCodeBySpace()</code> é usado internamente para dividir uma linha de código em tokens, facilitando o processamento de instruções. 
        </p>
        <p>
            No lado público, o construtor da classe inicializa a unidade de controle com um banco de registradores do tamanho especificado. Seu método principal é <code>ControlUnit::ULA()</code> (Unidade Lógica e Aritmética), o qual executa as instruções do bloco de memória do processo (MemoryPage), interagindo diretamente com a memória RAM e os registradores para processar operações lógicas e aritméticas. Em conjunto, esses elementos fazem da Unidade de Controle um componente central que orquestra a execução das operações no simulador, replicando o comportamento de uma CPU real.
        </p>
    </div>
    <div>
        <h3> 3) RAM </h3>
        <p>
            A classe <code>RamMemory</code> simula a memória do sistema, com um array de 32 inteiros, onde cada posição pode ser lida ou escrita por meio de funções de acesso. O estado da RAM pode ser exibido na saída padrão. A classe também implementa mecanismos para gerenciar os processos vindos da Memória ROM, nesse sentido ele são armazenados na fila de processos, onde ficam aguardando seu momento de execução. Esse gerenciamento acontece a partir de algumas funções indispensáveis:
            <ul>
                <li>
                    <code>RamMemory::RamMemory()</code>: O construtor da classe inicializa uma instância de memória RAM com um tamanho definido pelo parâmetro <i>size</i>. Sempre que o sistema inicializa, é verificado se o tamanho fornecido é maior que zero; caso contrário, lança uma exceção (invalid_argument) para garantir que a memória tenha uma capacidade válida. Em seguida, o construtor dimensiona o vetor memoryCells para comportar o número especificado de células de memória. Cada célula é inicializada com valores padrões zerados  Isso assegura que a RAM comece com todas as células devidamente alocadas e prontas para serem utilizadas, refletindo o estado inicial de uma memória limpa.
                </li>
                <li>
                    <code>RamMemory::write()</code>: A função de escrita da classe é responsável por alocar uma célula de memória (MemoryCell) na RAM, seguindo diferentes tipos de operações definidos pelo parâmetro <i>kindOfWrite</i>. No caso 0 (operação STORE), escreve no endereço especificado se ele estiver vazio ou ocupado pelo mesmo processo. No caso 1, limpa o endereço, marcando-o como disponível após a finalização de um processo. No caso 2, realiza um swap de registradores sujos para o primeiro espaço livre na memória; se não houver espaço, lança uma exceção indicando que a memória está cheia. A função também valida o endereço fornecido, garantindo que ele esteja dentro do intervalo permitido, e lança exceções em caso de erro, como endereço fora do intervalo ou tipo de escrita inválido. Essa função é essencial para gerenciar a alocação e liberação de células de memória no simulador, evitando conflitos e garantindo eficiência no uso da RAM.
                </li>
            </ul>
        </p>
    </div>
    <div>
        <h3>4) RegisterBank</h3>
        <p>
            A classe <code>RegistrarsBank</code> simula o banco de registradores simula o banco de registradores de uma Unidade de Controle, funcionando como um conjunto de 32 registradores utilizados para os valores durante a execução de instruções de um processo.
        </p>
        <p>
            Nesse sentido, para simular os componentes, a classe possui um vetor privado, onde é armazenados os registradores. Estes, por sua vez, é representado por um valor númerico inteiro e um estado lógico que indica se ele está ocupado ou sendo usado. Existe também atributo <i>Pc</i> (Program Counter) cujo é responsável por armazenar o endereço da próxima instrução a ser executada, simulando o fluxo de controle.
        </p>
    </div>
    <div>
        <h3>5) Recursos do Sistema</h3>
        <p>
            A classe <code>InputsOutputs</code> abstrai os periféricos de uma arquitetura computacional, permitindo o gerenciamento do uso concorrente desses dispositivos por diferentes processos. Ela funciona como uma tabela que armazena os periféricos – por padrão, o simulador possui apenas 5 periféricos disponíveis: Mouse, Teclado, Monitor, Impressora e Fone de Ouvido – e associa a cada um deles um par contendo o identificador do processo que o está utilizando e um estado booleano que indica se o periférico está ocupado. Em resumo, <code>InputsOutputs</code> facilita o controle de concorrência sobre os periféricos, garantindo que apenas um processo possa usar um dispositivo por vez, evitando conflitos.
        </p>
    </div>
    <div>
        <h3>6) Memória ROM</h3>
        <p>
            No simulador, a memória ROM não é implementada como uma classe, mas sim através de <b>arquivos de texto</b> (<code>.txt</code>) que armazenam os comandos a serem executados. Esses arquivos seguem uma convenção de nomenclatura padronizada: <code>codigo1.txt</code>, <code>codigo2.txt</code>, <code>codigo3.txt</code>, e assim por diante, até <code>codigoN.txt</code>. Essa estrutura permite que vários códigos/processos sejam executados de forma sequencial ou conforme a necessidade, bastando ajustar a variável que define a quantidade de códigos no arquivo <code>main.cpp</code> do sistema.
        </p>
        <p>
            Os códigos contidos na ROM devem seguir um padrão rígido para serem processados corretamente. O <b>primeiro requisito</b> é que a <b>primeira linha de cada arquivo</b> obrigatoriamente contenha o nome de um <b>periférico do sistema</b> (um dispositivo de entrada ou saída), já que todos os processos precisam interagir com algum recurso físico. Isso garante que cada processo seja associado a um dispositivo específico, como "Mouse", "Teclado" ou "Impressora". 
        </p>
        <p>
            Além disso, cada arquivo deve conter <b>uma única instrução por linha</b>, respeitando uma estrutura clara e legível. Essa abordagem garante que o simulador consiga interpretar cada comando de forma sequencial e sem ambiguidades. Não há limite para o número de instruções em cada arquivo, permitindo que processos de diferentes complexidades sejam representados.
        </p>
        <p>
            É importante ressaltar que a memória ROM do simulador funciona como um repositório de códigos em formato de texto, com um padrão de nomeação e estrutura que garante a correta associação dos processos a periféricos e a execução ordenada das instruções do sistema.
        </p>
    </div>
</section>
<section>
    <h2>📝 Instruções Definidas para a Memória ROM</h2>
    <div>
        <h3> 1. LOAD</h3>
        <ul>
            <li>
                <strong>Formato</strong>: LOAD &lt;valor&gt; &lt;dest&gt;
            </li>
            <li>
                <strong>Descrição</strong>: Carrega um valor imediato no registrador &lt;dest&gt.
            </li>
            <li>
                <strong>Exemplo</strong>: LOAD 10 16 ➡ Carrega o valor <i>int(10)</i> no registrador 16.
            </li>
        </ul>
    <div>
    <div>
        <h3>2. ADD</h3>
        <ul>
            <li>
                <strong>Formato</strong>: ADD &lt;src1&gt; &lt;src2&gt; &lt;dest&gt;
            </li>
            <li>
                <strong>Descrição</strong>: Soma o valor dos registradores &lt;src1&gt; e &lt;src2&gt; e armazena o resultado em &lt;dest&gt;.
            </li>
            <li>
                <strong>Exemplo</strong>: ADD 18 16 17 ➡ Soma o valor dos registradores 18 e 16 para armazenar o resultado no registrador 17.
            </li>
        </ul>
    </div>
    <div>
        <h3>3. SUB</h3>
        <ul>
            <li>
                <strong>Formato</strong>: SUB &lt;src1&gt; &lt;src2&gt; &lt;dest&gt;
            </li>
            <li>
                <strong>Descrição</strong>: Subtrai o valor em &lt;src2&gt; do valor em &lt;src1&gt; e armazena o resultado em &lt;dest&gt;.
            </li>
            <li>
                <strong>Exemplo</strong>: SUB 16 17 18 ➡ Subtrai o valor no registrador 17 do valor no registrador 16 e armazena o resultado em 18.
            </li>
        </ul>
    </div>
    <div>
        <h3>4. STORE</h3>
        <ul>
            <li>
                <strong>Formato</strong>: STORE &lt;src&gt; &lt;endereco&gt;
            </li>
            <li>
                <strong>Descrição</strong>: Armazena o valor do registrador &lt;src&gt; na posição de memória &lt;endereco&gt;.
            </li>
            <li>
                <strong>Exemplo</strong>: STORE 18 16 ➡ Armazena o valor do registrador 18 na posição de memória 16, se possível; Caso não, o processo entrará em estado de inanição.
            </li>
        </ul>
    </div>
    <div>
        <h3>5. IF</h3>
        <ul>
            <li>
                <strong>Formato</strong>: IF &lt;operation&gt; &lt;src1&gt; &lt;src2&gt; &lt;dest&gt;
            </li>
            <li>
                <strong>Descrição</strong>: Executa uma comparação entre os valores dos registradores &lt;src1&gt; e &lt;src2&gt;, com base na operação &lt;operation&gt; (&lt; ou &gt;). O resultado da comparação (6666 para verdadeiro, -6666 para falso) é armazenado no registrador &lt;dest&gt;.
            </li>
            <li>
                <strong>Exemplo</strong>: IF &gt; 10 20 30 ➡ Compara se o valor no registrador 10 é maior que o valor no registrador 20. O resultado (6666 ou -6666) será armazenado no registrador 30.
            </li>
        </ul>
    </div>
    <div>
        <h3>6. FOR</h3>
        <ul>
            <li>
                <strong>Formato</strong>: FOR &lt;operation&gt; &lt;times&gt; &lt;value&gt; &lt;dest&gt;
            </li>
            <li>
                <strong>Descrição</strong>: Realiza uma operação repetida (SUB ou ADD) um número de vezes especificado em &lt;times&gt;, usando o &lt;value&gt; como operando, e armazena o resultado acumulado no registrador &lt;dest&gt;.
            </li>
            <li>
                <strong>Exemplo</strong>: FOR ADD 5000 10 15 ➡ Realiza a operação de adição, somando o valor <i>int(10)</i> ao registrador 15 repetidamente por 5000 vezes.
            </li>
        </ul>
    </div>
</section>
<section>
    <h2>⚙️Como executar o simulador </h2>
    <p>
        O projeto possui um arquivo Makefile que realiza todo o procedimento de compilação e execução. Para tanto, temos as seguintes diretrizes de execução para distribuições Linux:
    </p>
    <div align="center">
        <table>
            <thead>
                <tr>
                    <th>Comando</th>
                    <th>Função</th>
                </tr>
            </thead>
            <tbody>
                <tr>
                    <td><code>make clean</code></td>
                    <td>Apaga a última compilação realizada contida na pasta build</td>
                </tr>
                <tr>
                    <td><code>make</code></td>
                    <td>Executa a compilação do programa utilizando o gcc, e o resultado vai para a pasta build</td>
                </tr>
                <tr>
                    <td><code>make run</code></td>
                    <td>Executa o programa da pasta build após a realização da compilação</td>
                </tr>
            </tbody>
        </table>
        <p align="center">
            <b>Tabela 1:</b> Comandos úteis para compilar e executar o programa de computador
        </p>
    </div>
</section>
<section>
    <h2>🌌 Arquitetura Multicore com Suporte a Concorrência</h2>
    <p>
        A solução foi projetada para simular uma arquitetura multicore com suporte à execução concorrente de processos. Para isso, utilizou-se a biblioteca padrão de threads em C++ (<code>std::thread</code>), permitindo que cada processo fosse executado de forma independente em um núcleo virtual. Os processos são gerenciados através de múltiplas threads, armazenadas em um vetor que permite o controle das suas execuções. Esse pseudoparalelismo foi integrado ao simulador, garantindo que múltiplos processos compartilhem os recursos do sistema de maneira segura, por meio de mecanismos de exclusão mútua (<code>std::mutex</code>) e sincronização. Dessa forma, a execução simultânea de processos, prevene condições de corrida e inconsistências nos dados compartilhados.
    </p>
    <p>
        Para garantir o uso eficiente do processador, implementou-se um sistema de preempção baseado em tempo quantum. Cada processo foi limitado a 35 ciclos de clock, com cada instrução consumida equivalendo a um clock. Essa estratégia assegura que nenhum processo monopolize a CPU, promovendo um compartilhamento justo dos recursos. Caso o tempo quantum de um processo seja esgotado antes de sua conclusão, ele é interrompido e devolvido à fila de processos por meio de um mecanismo de troca de contexto. Essa funcionalidade simula de forma realista o funcionamento de sistemas operacionais modernos, respeitando as regras de escalonamento baseadas no tempo.
    </p>
    <p>
        Nesse sentido, o ciclo de vida dos processos foi modelado conforme os estados descritos por Tanenbaum: <b>pronto</b>, <b>bloqueado</b> e <b>executando</b>. Processos que solicitam recursos ocupados, como dispositivos de entrada e saída, entram no estado bloqueado e aguardam até que o recurso seja liberado. Isso envolve o sistema de gerenciamento de recursos implementado na classe <code>InputsOutputs</code>, descrita anteriormente. Assim, se o recurso está ocupado, os processos bloqueados retornam para o final da fila de processos e ficam aguardando um novo ciclo de execução.
    </p>
    <p>
        Cada processo foi representado por uma struct que abstraí um bloco de controle de processos (PCB), denominado <code>MemoryPage</code>. Este PCB armazena informações essenciais, como o ID do processo, seu estado atual, as instruções associadas e os recursos necessários (como dispositivos de entrada e saída). O PCB também gerencia dados relacionados à memória e ao quantum restante de cada processo. Essa abstração facilitou a troca eficiente de contexto durante a preempção e a recuperação do estado de execução do processo.
    </p>
    <p>
        Para evitar problemas de concorrência durante a execução dos processos, foi utilizado um mecanismo de bloqueio por meio da classe <code>std::mutex</code>. Isso acontece na função <code>executeProcessInThread()</code>, a qual protege a região crítica onde o processo é executado, utilizando o recurso <code>lock_guard</code>. Isso garantiu que múltiplas threads não acessassem simultaneamente os mesmos recursos compartilhados, como memória ou dispositivos de entrada e saída. Dessa forma, foi possível assegurar a consistência dos dados e a integridade do sistema, mesmo em um ambiente com múltiplas threads concorrentes.
    </p>
    <p>
        Adicionalmente, a função <code>monitorProcesses()</code> foi implementada como um monitor central, responsável por gerenciar o ciclo de vida dos processos e distribuir os recursos do sistema. Esse monitor verifica periodicamente a fila de processos e despacha-os para execução em threads separadas. Ele também detecta quando um recurso está ocupado, devolvendo o processo à fila de processos e aguardando a próxima oportunidade de execução. Esse design modular permitiu que o sistema operacional simulado tomasse decisões dinâmicas e inteligentes, respeitando as regras de preempção e escalonamento.
    </p>
    <p>
        A solução também foi otimizada para garantir um uso eficiente dos recursos computacionais. O tempo total de execução do sistema é calculado em milissegundos para avaliar o desempenho do simulador. Além disso, a inclusão de um pequeno atraso (1 milissegundo) entre as verificações na função que monitora os processos ajudou a reduzir a sobrecarga do sistema, garantindo uma execução fluida sem comprometer o pseudoparalelismo.
    </p>
    <p>
        Conclui-se então que a implementação expande as capacidades do simulador para suportar arquiteturas multicore e preempção, proporcionando uma simulação robusta e realista de sistemas operacionais modernos. A utilização de threads para simular o paralelismo, combinada com mecanismos de exclusão mútua e gerenciamento eficiente de recursos, permitiu modelar um sistema que reflete fielmente os desafios e soluções de ambientes reais. Com o suporte ao ciclo de vida dos processos, ao gerenciamento de recursos e à troca de contexto, esta solução oferece uma base sólida para estudos e simulações avançadas de sistemas operacionais.
    </p>
</section>
<section>
    <h2>💻 Ferramentas Utilizadas</h2>
    <p>
        Durante o deselvovimento do algoritimo apresentado nesse repositório, foi-se utilizado de duas principais ferramentas de desenvolvimento: Visual Studio Code e WSL: Ubuntu.
    </p>
    <ul>
        <li>    
            <p>
                O Visual Studio Code é um editor de texto multiplataforma disponibilizado pela Microsoft para o desenvolvimento de aplicações, Conhecer essa ferramenta é importante para os desenvolvedores que pretendem trabalhar em ambientes multiplataforma, por exemplo,  podendo operar o desenvolvimento em ambiente Mac e Linux, ao mesmo tempo em que mantém o projeto compatível com Windows. Segue em anexo o link para o site da plataforma — <a href="https://code.visualstudio.com/" target="_blank">Visual Studio Code</a>.
            </p>
        </li>
        <li>
            <p>
                O WSL é um método para se executar comandos Linux dentro do Windows de forma simplificada. Essa platoforma se torna essencial para o desenvovimento, evitando tarefas como a intalação de máquinas vituais e a criação de dual booting no computador pessoal. Nesse projeto, o WSL foi utilizado para compilar e executar o programa em C++ em um ambiente Linux através do Windows. Segue em anexo um  tutorial de download do WSL2 — <a href="https://youtu.be/hd6lxt5iVsg" target="_blank">Tutorial WSL2</a>.
            </p>
        </li>
    </ul>
</section>
<section>
    <h2>📚 Referências</h2>
    <p>
        BERTINI, Luciano. <strong>Capítulo 5 Pipeline</strong>. 2019. Disponível em: <a href="https://www.professores.uff.br/lbertini/wp-content/uploads/sites/108/2017/08/Cap-5-Pipeline.pdf">https://www.professores.uff.br/lbertini/wp-content/uploads/sites/108/2017/08/Cap-5-Pipeline.pdf</a>. Acesso em: 4 jan. 2025.
    </p>
</section>


