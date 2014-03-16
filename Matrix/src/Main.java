import java.io.*;

public class Main {
	public static void main(String[] args) throws IOException {
		FillingMatrix fillingByRead = new FillingMatrix("console");

		Matrix matrix1 = new Matrix(2);
		Matrix matrix2 = new Matrix(2);
		Matrix matrixResult = new Matrix(2);

		// заполн€ем первую матрицу с консоли
		matrix1.fill(fillingByRead);

		// находим и печатаем детерминант
		System.out.print(MatrixFunction.calculateDeterminant(matrix1));

		matrix1.printMatrix();

		// выводим матрицу, приведенную к трегульному виду
		MatrixFunction.gauss(matrix1).printMatrix();

		// заполн€ем вторую матрицу из файла(лежащего в текущей директории, если
		// нет, то через полное им€ файла)
		FillingMatrix fillingFromFile = new FillingMatrix("new.txt");

		matrix2.fill(fillingFromFile);

		// »щем сумму. “.к. MatrixFunction - мо€ библиотека, то она не ловит
		// ошибки, их ловит Main
		try {
			matrixResult = MatrixFunction.sum(matrix1, matrix2);
			matrixResult.printMatrix();
		} catch (IllegalStateException e2) {
			System.out.println(e2.getMessage());
		}

		matrix1.setValue(matrix1.getValue());
		matrix2.setValue(matrix2.getValue());
		// ищем произведение
		try {
			matrixResult = MatrixFunction.multiplication(matrix1, matrix2);
			matrixResult.printMatrix();
		} catch (IllegalStateException e1) {
			System.out.println(e1.getMessage());
		}
	}
}
