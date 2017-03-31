/*
*  	 Word ladder
* -----------------
*-> Çizgeyi saklamak için adjacency linked list veri yapısı kullanıldı.
*-> En kısa yolu bulmak için BFS algoritması kullanıldı. (Graph sınıfı içerisinde findShortestPath  fonksiyonu)
*
* adjList: Dictionary.txt deki toplam kelime uzunluğunda, her kelimenin sırasıyla indexlendiği dizi.
*			Dizinin elemanları Vertex obje referansından oluşur,
*			komşular ise her eleman için o elemana bağlı Adjacency sınıfı tipinden bağlı liste ile temsil edilir.
*/

import java.io.*;
import java.util.Scanner;

public class Main {

	public static void main(String[] args) throws IOException {
		FileProcess files = new FileProcess(args[0], args[1], args[2]);
		Graph g1 = new Graph();
		Vertex[] adjList = new Vertex[files.getLineNumber()];		// kelime sayısını bul.
		String[] gameWords = new String[2];
		int pos1 = 0, pos2 = 0;
		
		files.setVertexes(adjList);			// köşeleri diziye at.
		files.openFiles();
		g1.setGraph(adjList);				// graphı oluştur.
		while( (gameWords = files.getVertexes()) != null ) {	// input dosyasını satır satır oku
			pos1 = g1.findVertexNumber(gameWords[0], adjList);		// kelimenin adjList dizisindeki pozisyonunu bul
			pos2 = g1.findVertexNumber(gameWords[1], adjList);
			files.write(g1.findShortestPath(pos1, pos2, adjList));		// en kısa yolu dosyaya yaz
		}	
		files.closeFiles();	
	}	
}
