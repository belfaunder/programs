public class Main 
{
	public static void main(String[] args) 
	{	
		FillingMatrix fillingByRead = new FillingMatrix("read");
		//int size = fillingByRead.in.nextInt();
		Matrix matrix1 = new Matrix(4);
		Matrix matrix2 = new Matrix(4);
		Matrix matrixResult = new Matrix(4);
		

		
		matrix1.getVal(fillingByRead, matrix1);
		System.out.print(MatrixFunction.calculateDeterminant(matrix1));
		MatrixFunction.gauss(matrix1).printMatrix();
		//matrix2.getVal(fillingByRead, matrix2);
		
		//matrixResult = MatrixFunction.Sum(matrix1, matrix2);
		//matrixResult.printMatrix();
		
		
	}

}
