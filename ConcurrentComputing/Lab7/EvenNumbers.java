import java.sql.Time;
import java.util.Random;
import javax.naming.spi.DirStateFactory.Result;

/*
Há 2 diferenças principais entre "implements Runnable" e "extends Thread".

A mais comum é:

- "extends Thread" gasta sua herança, tal que não é possível
  herdar de outras classes

- "implements Runnable" permite que sua classe herde de outras,
  mas ainda se beneficie do multithreading

A mais significativa é:

- "extends Thread" faz com que cada thread crie um objeto
  para si e se associe a ele

- "implements Runnable" compartilha o mesmo objeto criado
  entre todas as threads
*/


// Classe que realiza a checagem concorrente da qtde. de pares
class EvenCounter extends Thread {
    // Recurso único a cada thread
    private int threadId;

    // Recursos compartilhados por todas as threads
    private static int nThreads = 0;
    private static int[] numbersArray;
    private static ResultHolder holder;

    // Construtor
    public EvenCounter(int threadId, int[] numbersArray, int nThreads, ResultHolder holder) {
        this.threadId = threadId;
        EvenCounter.numbersArray = numbersArray;
        EvenCounter.nThreads = nThreads;
        EvenCounter.holder = holder;
    }

    // Função executada pelas threads
    public void run() {
        // Calcula se é par; se sim, atualiza o resultado
        for (int i = this.threadId; i < EvenCounter.numbersArray.length; i += EvenCounter.nThreads) {
            if (numbersArray[i] % 2 == 0)
                ResultHolder.updateResult();
        }
    }
}


/*
   Classe que anota os resultados

   Como ela é "final" e os seus atributos são "static",
   ela equivale a uma "static class", o que não existe
   para classes de top-level
*/
final class ResultHolder {
    private static int counter = 0;

    public static synchronized void updateResult() {
        ResultHolder.counter++;
    }

    public static int getResult() {
        return ResultHolder.counter;
    }
}

// Classe do main
public class EvenNumbers {
    public static void main(String[] args) {    
        // Cria variáveis de thread
        final int nThreads = 7;
        Thread[] threadsArray = new Thread[nThreads];

        // Cria variáveis de números
        final int arraySize = 250000;
        int[] numbersArray = new int[arraySize];
        int counter = 0;

        // Cria e definindo variável de checagem
        ResultHolder holder = new ResultHolder();

        // Define valores pro array de números
        Random numberGenerator = new Random(System.currentTimeMillis());
        for (int i = 0; i < numbersArray.length; i++) {
            numbersArray[i] = numberGenerator.nextInt();

            if (numbersArray[i] % 2 == 0) {
                counter++;
            }
        }

        // Cria as threads
        for (int i = 0; i < threadsArray.length; i++) {
            threadsArray[i] = new Thread(new EvenCounter(i, numbersArray, nThreads, holder));
        }

        // Inicia as threads
        for (int i = 0; i < threadsArray.length; i++) {
            threadsArray[i].start();
        }

        // Espera o término de todas as threads
        for (Thread t : threadsArray) {
            try {
                t.join();
            }
            catch (InterruptedException e) {
                return;
            }
        }

        // Compara os resultados
        if (counter == ResultHolder.getResult()) {
            System.out.println("Correto!\nO número de pares calculado pelas threads é "
            + ResultHolder.getResult() + ";\nO número de pares real é "
            + counter + ".\n");
        } else {
            System.out.println("Incorreto.\nO número de pares calculado pelas threads é "
                                + ResultHolder.getResult() + ";\nO número de pares real é "
                                + counter + ".\n");
        }
    }
}