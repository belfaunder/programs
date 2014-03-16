import java.io.DataInputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.Scanner;

public class FillingMatrix {
	public String wayOfFilling;

	public FillingMatrix(String way) {
		wayOfFilling = way;
	}

	// заполняет матрицу, считывая значения с терминала
	public void read(Matrix matrix) {
		int size = matrix.getSize();
		boolean flag = true;
		int[][] buffer = new int[size][size];
		try {
			Scanner scanner = new Scanner(System.in);
			for (int i = 0; i < size; i++) {
				for (int j = 0; j < size; j++) {
					while (flag) {
						buffer[j][i] = scanner.nextInt();
						flag = false;

					}
					flag = true;
				}
			}
		} catch (java.util.InputMismatchException exception) {
			System.out.println("Введите число");
		}
		matrix.setValue(buffer);
	}

	// заполняет матрицу из файла
	public void fromFile(Matrix matrix) throws IOException,
			FileNotFoundException {
		String path = this.wayOfFilling;

		int size = matrix.getSize();
		int[][] buffer = new int[size][size];
		File file = new File(path);
		boolean exists = file.exists();
		try {
			if (!exists) {
				throw new FileNotFoundException("Такого файла не существует");
			}
			FileInputStream inputStream = new FileInputStream(path);
			@SuppressWarnings("resource")
			Scanner scan = new Scanner(inputStream);
			for (int i = 0; i < size; i++) {
				for (int j = 0; j < size; j++)
					buffer[j][i] = scan.nextInt();
			}
			matrix.setValue(buffer);
			inputStream.close();
		}

		catch (FileNotFoundException e1) {
			System.out.println(e1.getMessage());
		} catch (java.util.InputMismatchException e2) {
			System.out.println("Исходный файл заполнен неверно");
		} catch (java.util.NoSuchElementException e3) {
			System.out.println("Исходный файл заполнен неверно");
		}
	}

}
