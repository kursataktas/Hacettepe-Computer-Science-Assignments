/*
*	Graph fonksiyonları.
*/

import java.awt.List;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.Map;
import java.util.Queue;

public class Graph {
	
	public boolean compareCharByChar (Vertex v1, Vertex v2) {
		/* Verilen 2 string arasında 1 karakterlik fark varsa true, diğer durumlarda false döndürür */
		
		char[] x = v1.name.toCharArray();
		char[] y = v2.name.toCharArray();
		int counter = 0;
		
		for (int i = 0; i < y.length; i++) {
			if (x[i] != y[i])
				counter++;
		}
		if(counter == 1)
			return true;
		else
			return false;
	}
	
	public void setGraph(Vertex[] adjList) {
		/* verilen köşe dizisi için adjacency linked list veriyapısı tipinde graphı kurar */
		
		for (int i = 0; i < adjList.length; i++) {
			for (int j = 0; j < adjList.length; j++) {
				if (compareCharByChar(adjList[i], adjList[j]) && !(adjList[i].name.equals(adjList[j].name))) {
					adjList[i].adj = new Adjacency(j, adjList[i].adj);
				}
			}
		}
	}
	
	public String findShortestPath(int begin, int end, Vertex[] adjList ) {
		/* input: başlangıç ve bitiş kelimelerinin köşe dizisindeki indexleri
			çıktı: eğer yol varsa yolu, yoksa hatayı string tipinde döndürür
		*/
		
		boolean[] visited = new boolean[adjList.length];
		Map<String,String> trace = new HashMap<String,String>();
		Queue<Integer> que = new LinkedList<Integer>();
		int cur = 0;
		
		visited[begin] = true;
		que.add(begin);
		while (!que.isEmpty()) {
			cur = (int) que.remove();
			if(cur == end)
				break;
			else {
				for (Adjacency adjs = adjList[cur].adj; adjs != null; adjs = adjs.next) {
					if(!visited[adjs.vNum]) {
						que.add(adjs.vNum);
						visited[adjs.vNum] = true;
						trace.put(adjList[adjs.vNum].name, adjList[cur].name);
					}
				}
			}
		}
		if(cur != end)
			return "Failure";
		else {
			LinkedList<String> pathList = new LinkedList<String>();
			String path = "";
			for (String vName = adjList[end].name; vName != null; vName = trace.get(vName)) 
				pathList.add(vName);
			for (int i = pathList.size()-1; i >= 0; i--) 
				path += pathList.get(i) + ",";
			return path.substring(0, path.length()-1);
		}
	}
	
	public int findVertexNumber(String vName, Vertex[] adjList) {
		/* verilen kelimenin köşe dizisindeki indexini döndürür */
		
		int i = 0;
		for (i = 0; i < adjList.length; i++) {
			if(vName.equals(adjList[i].name))
				break;
		}
		if (i != adjList.length) {
			return i;
		} else {
			return 0;
		}
	}
}
