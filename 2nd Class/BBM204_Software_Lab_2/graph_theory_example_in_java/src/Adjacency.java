/*
*	Vertex'in komşularını gösterirken kullanılan sınıf. 
*		vNum komşunun vertex dizisindeki pozisyonunu temsil eder.
*		Komşu listesine yeni komşu bağlama işlemi kurucuda gerçekleşir.
*/
public class Adjacency {
	public int vNum;
	public Adjacency next;
	
	public Adjacency(int num, Adjacency adj) {
		this.vNum = num;
		this.next = adj;
	}
}
