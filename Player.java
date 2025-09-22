import java.util.Random;

public class Player {

    private String nome;
    private int jogada;
    private int vitoria;
    private Random randomGenerator; //gerador de números randômicos

    public Player(String nome){ //construtor dos jogadores
        this.nome = nome;
        this.jogada = 0;
        this.randomGenerator = new Random();
    }
    public void fazerJogada(){
        this.jogada = randomGenerator.nextInt(3); //num aleatorio de 0 a 2 (3 jogadas)
    }
    public void somarVitorias(){
        this.vitoria++;
    }

    public String getNome(){
        return this.nome;
    }

    public int getJogada(){
        return this.vitoria;
    }

}
