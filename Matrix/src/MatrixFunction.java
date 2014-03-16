public class MatrixFunction 
{	
	public static class GaussMatrix extends Matrix
	{	private int determinant;
		int getDeterminant()
		{	return determinant;
		}
		public GaussMatrix(int size, int [][] matrix, int det) 
		{	super(size, matrix);
			determinant = det;
		}
	}
	
	//ищет сумму двух матриц
	public static Matrix sum( Matrix matrix1, Matrix matrix2) throws IllegalStateException
	{	if(matrix1.getSize() != matrix2.getSize())
		{	throw new IllegalStateException("Размеры матриц не совпадают");
		}
	
		int size = matrix1.getSize();
		Matrix matrixResult = new Matrix(size);
		int [][] result = new int [size][size];
		int [][] m1 = matrix1.getValue();
		int [][] m2 = matrix2.getValue();
		for( int i = 0; i < size; i++)
		{	for(int j = 0; j < size; j++)
			{	
				result[i][j] = m1[i][j] + m2[i][j];
			}
		}
		matrixResult.setValue(result);
		return(matrixResult);
	}
	
	//ищет произведение двух матриц
	public static Matrix multiplication(Matrix matrix1, Matrix matrix2) throws IllegalStateException
	{	if(matrix1.getSize() != matrix2.getSize())
		{	throw new IllegalStateException("Размеры матриц не совпадают");
		}
	
		int size = matrix1.getSize();
		int [][] result = new int [size][size];
		int [][] operand1 = matrix1.getValue();
		int [][] operand2 = matrix2.getValue();
		
		for( int i = 0; i < size; i++)
		{	for(int j = 0; j < size; j++)
			{	result[i][j] = 0;
			}
		}
		
		for (int k=0; k < size; k++)
		{	for(int i = 0; i < size; i++)
	 		{	for(int j=0; j < size; j++)
                    result[i][j] += operand1[i][k]*operand2[k][j];
            }
	 	}
		
		Matrix matrixResult = new Matrix(size, result);
	 	return(matrixResult);
	}
	
	//ищет lcm(нок)
	private static int lcm(int operand1, int operand2)
	{	int result = operand1 * operand2 / gcd(operand1, operand2);
		return result;
	}
	
	//ищет gcd(нод)
	private static int gcd(int operand1, int operand2)
	{
		int a = operand1;
		if (a<0) a = 0 - a;
		int b = operand2;
		if (b<0) b = 0 - b;
		while((a!=0) && (b!=0))
		{	if(a>=b) a=a%b;
			else b=b%a;
		}
		a=a+b;	
		return a;
	}
	
	//создет гаусову матрицу из данной
	public static GaussMatrix gauss (final Matrix matrixArg)
	{	Matrix argument = matrixArg.matrixCopy();
		int size = argument.getSize();
		int denominator = 1;
		int multiplier1;
		int multiplier2;
		int numerator = 1;
		int [][] matrix = argument.getValue();
		for (int i=0; i<size; i++)
		{	if (matrix[i][i] == 0)
			{	int j = i;
				while ((j<size) && (matrix [i][j] == 0))
					j++;
				if (j != size)
				{	int buffer;
					for (int p=i; p<size; p++)
					{	buffer = matrix[p][i];
						matrix[p][i] = matrix[p][j];
						matrix[p][j] = buffer;
					}
					numerator = - numerator;
				}
			}
			for (int j=i+1; j<size; j++)
			{
				if (matrix [i][i] == 0)
				{	multiplier2 = 1;
					multiplier1 = 0;
				}
				else
				{	int lcm = lcm(matrix[i][i], matrix[i][j]);
					multiplier1 = lcm / matrix[i][i];
					if (matrix[i][j]!=0) 
						multiplier2 = lcm / matrix[i][j];
					else
						multiplier2 = 1;
					denominator = denominator * multiplier2;
				}
				for (int k=i; k<size; k++)
				{	matrix[k][j] = multiplier2 * matrix[k][j] - multiplier1 * matrix[k][i];
				}
			}
			numerator = numerator * matrix[i][i];	
		}
		GaussMatrix gaussMatrix = new GaussMatrix(size, matrix, numerator/denominator);
		return gaussMatrix;
	}
	
	//ищем детермиант
	public static int calculateDeterminant( Matrix argument)
	{	GaussMatrix gaussMatrix = gauss(argument);
		return gaussMatrix.getDeterminant();
	}
}
