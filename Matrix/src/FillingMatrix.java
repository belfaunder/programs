import java.util.*;

public class FillingMatrix 
{	public String wayOfFilling;	
	
	public Scanner in = new Scanner(System.in);
	
	public FillingMatrix(String way)
	{	wayOfFilling = way;
	}
	
	public void read(Matrix matrix)
	{	
		int size = matrix.getSize();
		int [][] buffer = new int [size][size];
		for( int i = 0; i < size; i++)
		{	for(int j = 0; j < size; j++)
			{	buffer[j][i] = in.nextInt();
			}
		}
		matrix.setValue(buffer);
	}
}
