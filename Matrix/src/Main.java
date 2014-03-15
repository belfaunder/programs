
public class Main 
{
	public static void main(String[] args) 
	{			
		FillingMatrix fillingByRead = new FillingMatrix("read");
		
		Matrix matrix1 = new Matrix(2);
		Matrix matrix2 = new Matrix(3);
		Matrix matrixResult = new Matrix(2);
		
		matrix1.getVal(fillingByRead, matrix1);	
		matrix2.getVal(fillingByRead, matrix2);
		
//		System.out.print(MatrixFunction.calculateDeterminant(matrix1));
	//	MatrixFunction.gauss(matrix1).printMatrix();
		//matrix2.getVal(fillingByRead, matrix2);
		
		try
		{	matrixResult = MatrixFunction.Sum(matrix1, matrix2);	
			matrixResult.printMatrix();
		}
		
		catch( IllegalStateException e)
		{	System.out.println(e.getMessage());			
		}

		
	}
}
