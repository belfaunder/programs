import java.io.IOException;

public class Matrix 
{	private int size;
	private int [][] value;
	
	public Matrix( int asize)
	{	size = asize;
		value = new int [size][size];
	}
	
	public Matrix (int newSize, int [][] newValue)
	{	size = newSize;
		value = newValue;
	}
	
	public void changeRaws(int number1, int number2)
	{
		int buffer;
		for (int i=0; i<size; i++)
		{
			buffer = value[i][number1];
			value[i][number1] = value[i][number2];
			value[i][number2] = buffer;
		}
	}
	
	public int getSize()
	{		return size;
	}
	
	public void setValue(int [][] newValue)
	{		value = newValue;
	}
	
	public int [][] getValue()
	{		return value;
	}
	
	public void fill( FillingMatrix filling) throws IOException
	{	if ( filling.wayOfFilling.equals("console") )
		{	filling.read(this);
		}
		else
		{	filling.fromFile(this);		
		}
	}
	
	public void printMatrix()
	{	System.out.println();
		for( int i = 0; i < this.size; i++)
		{	for(int j = 0; j < this.size; j++)
			{	 System.out.print(this.value[j][i]+" ");
			}
			System.out.println();
		}		
	}	
	public Matrix matrixCopy()
	{	int size = this.size;
		Matrix matrix = new Matrix(size);
		int [][] matrix1 = matrix.getValue();
		int [][] matrix2 = this.getValue();
		for(int i = 0; i < size*size; i++)
		{	
			matrix1[i%size][i/size] = matrix2[i%size][i/size];
		}
		matrix.setValue(matrix1);
		return matrix;
	}
}
