package decodeLZ4;

public class historyTest {


	final static int hbMargin=32;
	private static int hbSize = 64;				//rozmiar bufora historii
	private static int bSize = hbSize + hbMargin;	//rozmiar rzeczywisty bufora,	
	private static int hbHead=0;				//wskaznik zapisu danych, opiera sie na wskaznikach BSIZE oraz HBFLUSH
	private static int hbTail=0;				//okresla dolna granice bufora historii HBHEAD - HBTAIL == HBSIZE,
												//jest to HEAD dla HBFLUSH
	private static int hbFlush=0;				//dane sfluszowane do OBUFF-a
	private byte[] hBuffer;
	

	

	public historyTest(int bufferSize){
		hbSize = bufferSize;
		bSize = hbSize + hbMargin; 
		this.hBuffer = new byte[bSize];
		System.out.println("@INF: Utworzony: HistoryBuffer["+hbSize+"]+["+hbMargin+"]");
	};
		
	
}
