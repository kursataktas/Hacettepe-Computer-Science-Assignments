/*
*	Dosya açma,okuma,yazma,kapama işlemleri ve grapha özel dosya ile bağıntılı bazı fonksiyonları içerir
*/
import java.io.*;
import java.util.Scanner;
public class FileProcess {
	
	String dictionary;
	String input;
	String output;
	Scanner scInput = null;
	PrintWriter writer = null;
	public FileProcess(String dic, String in, String out) {
		dictionary = dic;
		input	   = in;
		output 	   = out;
	}
	
	public int getLineNumber() throws IOException {
		/* dictionary dosyanın toplam satır uzunluğunu çıktı olarak döndürür.
			NOT:eğer dosyada ingilizce dışı kelimelerden oluşan bir satır varsa bu satır ele alınmaz.
		*/
		
		Scanner scDictionary = new Scanner(new File(this.dictionary));
		boolean isEnglish;
		String pattern = "\\w+";
		int i = 0;
		for(i = 0; scDictionary.hasNext(); i++) {
			String word = scDictionary.next();
			if(! (isEnglish = word.matches(pattern)))
				i--;
		}
		scDictionary.close();
		return i;
	}
	
	public void setVertexes(Vertex adjList[]) throws IOException {
		/* Verilen boş diziye dictionary.txt deki her kelimeyi indexler. */
		
		Scanner scDictionary = new Scanner(new File(this.dictionary));
		boolean isEnglish;
		String pattern = "\\w+";
		for(int i = 0; scDictionary.hasNext(); i++) {
			String word = scDictionary.next();
			if(isEnglish = word.matches(pattern)){
				adjList[i] = new Vertex(word, null);
			}
			else
				i--;
		}
		scDictionary.close();
	}
	
	public String[] getVertexes() throws IOException{
		/* input dosyanından başlangıc ve bitiş kelimelerini string dizisi şeklinde döndürür.
			ilk eleman başlangıç, ikinci eleman bitiş kelimesi 
		*/
		
		String vertexes[] = new String[2];
		
		if(this.scInput.hasNext()){
			vertexes[0] = scInput.next();
			vertexes[1] = scInput.next();
			return vertexes;
		}
		else
			return null;
	}
	
	public boolean openFiles() throws IOException {
		scInput = new Scanner(new File(this.input));
		writer  = new PrintWriter(output,"UTF-8");
		
		if (scInput != null && writer != null) 
			return true;
		else 
			return false;
	}
	
	public void write(String text) {
		writer.println(text);
	}
	
	public void closeFiles() {
		if(scInput != null)
			scInput.close();
		if(writer != null)
			writer.close();
	}
}
