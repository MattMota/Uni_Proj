import java.time.Duration;
import java.time.Instant;
import java.util.Random;

//--------------------------------------------------------
// Classe principal
class LeitorEscritor {
    
    static final int T1_Counter = 10;
    static final int T2_Counter = 11;
    static final int T3_Counter = 12;

    public static void main (String[] args) {
        // Variáveis de execução
        Var var = new Var(0);
        Monitor monitor = new Monitor();

        // Execução propriamente dita
        T1[] T1_Array = new T1[T1_Counter]; // Threads T1
        T2[] T2_Array = new T2[T2_Counter]; // Threads T2
        T3[] T3_Array = new T3[T3_Counter]; // Threads T3

        // Inicia o log de saida
        System.out.println ("import verificaLE");
        System.out.println ("le = verificaLE.LE()");
        
        for (int i = 0; i < T1_Counter; i++) {
            T1_Array[i] = new T1(i, monitor, var);
            T1_Array[i].start();
            i++;
        }

        for (int i = 0; i < T2_Counter; i++) {
            T2_Array[i] = new T2(i, monitor, var);
            T2_Array[i].start();
            i++;
        }

        for (int i = 0; i < T3_Counter; i++) {
            T3_Array[i] = new T3(i, monitor, var);
            T3_Array[i].start();
            i++;
        }
    }
}

//--------------------------------------------------------
// Classes das threads (precisam estar definidas
// no main para não dar problema de escopo ao acessar
// var e monitor)
//
// Fiz cada thread rodar por 5 segundos para
// haver bastante interação entre elas (Instant
// e Duration são thread-safe segundo doc. do Java)

class T1 extends Thread {
    private int id;
    private Monitor monitor;
    private Var var;

    public T1(int id, Monitor monitor, Var var) {
        this.id = id;
        this.monitor = monitor;
        this.var = var;
    }

    // Roda por 5 segundos
    public void run() {
        Instant start = Instant.now();
        Instant finish;
        do {
            finish = Instant.now();
            monitor.EntraEscritor("\"" + this.getClass().getSimpleName() + ", id " + this.id + "\"");
            var.setVar(var.getVar()+1);
            monitor.SaiEscritor("\"" + this.getClass().getSimpleName() + ", id " + this.id + "\"");
        } while(Duration.between(start, finish).toSeconds() < 5);
    }
}

class T2 extends Thread {
    private int id;
    private Monitor monitor;
    private Var var;

    public T2(int id, Monitor monitor, Var var) {
        this.id = id;
        this.monitor = monitor;
        this.var = var;
    }

    // Roda por 5 segundos
    public void run() {
        Instant start = Instant.now();
        Instant finish;
        do {
            finish = Instant.now();
            monitor.EntraLeitor("\"" + this.getClass().getSimpleName() + ", id " + this.id + "\"");
            if (var.getVar() % 2 == 0) {
                System.out.println("print(\""+ var.getVar() + " e par\")");
            } else {
                System.out.println("print(\""+ var.getVar() + " e impar\")");
            }
            monitor.SaiLeitor("\"" + this.getClass().getSimpleName() + ", id " + this.id + "\"");
        } while(Duration.between(start, finish).toSeconds() < 5);
    }
}

class T3 extends Thread {
    private int id;
    private Monitor monitor;
    private Var var;

    public T3(int id, Monitor monitor, Var var) {
        this.id = id;
        this.monitor = monitor;
        this.var = var;
    }

    // Roda por 5 segundos
    public void run() {
        Instant start = Instant.now();
        Instant finish;
        do {
            finish = Instant.now();
            monitor.EntraLeitor("\"" + this.getClass().getSimpleName() + ", id " + this.id + "\"");
            System.out.println("print(\"Var = " + var.getVar() + "\")");
            monitor.SaiLeitor("\"" + this.getClass().getSimpleName() + ", id " + this.id + "\"");

            monitor.EntraEscritor("\"" + this.getClass().getSimpleName() + ", id " + this.id + "\"");
            for (int i = 100000; i > 0; i--) {
                Random r = new Random();
                int gastatempo = (r.nextInt(i)+1)/(id+1);
            }
            var.setVar(this.id);
            monitor.SaiEscritor("\"" + this.getClass().getSimpleName() + ", id " + this.id + "\"");
        } while(Duration.between(start, finish).toSeconds() < 5);
    }
}

//--------------------------------------------------------
// Monitor que implementa a logica do padrao leitores/escritores

class Var {
    private int var;

    Var(int var) {
        this.var = var;
    }

    public void setVar(int var){
        this.var = var;
    }

    public int getVar() {
        return this.var;
    }
}

//--------------------------------------------------------
// Monitor que implementa a logica do padrao leitores/escritores
class Monitor {
  private int leit, escr;
  
  // Construtor
  Monitor() { 
     this.leit = 0; //leitores lendo (0 ou mais)
     this.escr = 0; //escritor escrevendo (0 ou 1)
  }
  
  // Entrada para leitores
  public synchronized void EntraLeitor (String id) {
    try { 
      while (this.escr > 0) {
      //if (this.escr > 0) {
         System.out.println ("le.leitorBloqueado("+id+")");
         wait();  //bloqueia pela condicao logica da aplicacao 
      }
      this.leit++;  //registra que ha mais um leitor lendo
      System.out.println ("le.leitorLendo("+id+")");
    } catch (InterruptedException e) { }
  }
  
  // Saida para leitores
  public synchronized void SaiLeitor (String id) {
     this.leit--; //registra que um leitor saiu
     if (this.leit == 0) 
           this.notify(); //libera escritor (caso exista escritor bloqueado)
     System.out.println ("le.leitorSaindo("+id+")");
  }
  
  // Entrada para escritores
  public synchronized void EntraEscritor (String id) {
    try { 
      while ((this.leit > 0) || (this.escr > 0)) {
      //if ((this.leit > 0) || (this.escr > 0)) {
         System.out.println ("le.escritorBloqueado("+id+")");
         wait();  //bloqueia pela condicao logica da aplicacao 
      }
      this.escr++; //registra que ha um escritor escrevendo
      System.out.println ("le.escritorEscrevendo("+id+")");
    } catch (InterruptedException e) { }
  }
  
  // Saida para escritores
  public synchronized void SaiEscritor (String id) {
     this.escr--; //registra que o escritor saiu
     notifyAll(); //libera leitores e escritores (caso existam leitores ou escritores bloqueados)
     System.out.println ("le.escritorSaindo("+id+")");
  }
}