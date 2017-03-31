/*
*	Kelimelerin saklandığı sınıf. sınıfın Adjacency sınıfına referansı vardır. 
*	Graphdaki vertexin komşuları Adjacency sınıfı tipinden objelerle temsil edilir.
*/
public class Vertex {
	String name;
	Adjacency adj;
	
	public Vertex(String name, Adjacency adj) {
		this.name = name;
		this.adj = adj;
	}
	
	public String toString() {
		return this.name + " ";
	}
}
