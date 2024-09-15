package Client;

public class TPUser {
    private String username;
    private int nivel;
    private int vitorias;
    private int derrotas;

    public TPUser(String username, int nivel, int vitorias, int derrotas) {
        this.username = username;
        this.nivel = nivel;
        this.vitorias = vitorias;
        this.derrotas = derrotas;
    }

    // Getters
    public String getUsername() {
        return username;
    }

    public int getNivel() {
        return nivel;
    }

    public int getVitorias() {
        return vitorias;
    }

    public int getDerrotas() {
        return derrotas;
    }

    // Setters
    public void setNivel(int nivel) {
        this.nivel = nivel;
    }

    public void setVitorias(int vitorias) {
        this.vitorias = vitorias;
    }

    public void setDerrotas(int derrotas) {
        this.derrotas = derrotas;
    }

    @Override
    public String toString() {
        return username + ": Nível " + nivel + ", Vitórias " + vitorias + ", Derrotas " + derrotas;
    }
}

