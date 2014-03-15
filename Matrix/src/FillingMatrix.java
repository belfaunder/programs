import java.util.*;

public class FillingMatrix 
{	public String wayOfFilling;	
	
	public FillingMatrix(String way)
	{	wayOfFilling = way;
	}
	//��������� �������, �������� �������� � ���������
	public void read(Matrix matrix)
	{	
		int size = matrix.getSize();
		boolean flag = true;
		int [][] buffer = new int [size][size];
		for( int i = 0; i < size; i++)
		{	for(int j = 0; j < size; j++)
			{	while( flag )
				{	try
					{	buffer[j][i] = (new Scanner(System.in)).nextInt();
						flag = false;
					}
					catch(  java.util.InputMismatchException exception)
					{	System.out.println("������� �����");
					}
				}
				flag = true;
			}
		}
		matrix.setValue(buffer);
	}
	//��������� ������� �� �����
	public void fromFile(Matrix matrix)
	{	
	}
	
}
