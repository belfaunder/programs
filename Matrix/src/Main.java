import java.io.*;

public class Main {
	public static void main(String[] args) throws IOException {
		FillingMatrix fillingByRead = new FillingMatrix("console");

		Matrix matrix1 = new Matrix(2);
		Matrix matrix2 = new Matrix(2);
		Matrix matrixResult = new Matrix(2);

		// ��������� ������ ������� � �������
		matrix1.fill(fillingByRead);

		// ������� � �������� �����������
		System.out.print(MatrixFunction.calculateDeterminant(matrix1));

		matrix1.printMatrix();

		// ������� �������, ����������� � ����������� ����
		MatrixFunction.gauss(matrix1).printMatrix();

		// ��������� ������ ������� �� �����(�������� � ������� ����������, ����
		// ���, �� ����� ������ ��� �����)
		FillingMatrix fillingFromFile = new FillingMatrix("new.txt");

		matrix2.fill(fillingFromFile);

		// ���� �����. �.�. MatrixFunction - ��� ����������, �� ��� �� �����
		// ������, �� ����� Main
		try {
			matrixResult = MatrixFunction.sum(matrix1, matrix2);
			matrixResult.printMatrix();
		} catch (IllegalStateException e2) {
			System.out.println(e2.getMessage());
		}

		matrix1.setValue(matrix1.getValue());
		matrix2.setValue(matrix2.getValue());
		// ���� ������������
		try {
			matrixResult = MatrixFunction.multiplication(matrix1, matrix2);
			matrixResult.printMatrix();
		} catch (IllegalStateException e1) {
			System.out.println(e1.getMessage());
		}
	}
}
