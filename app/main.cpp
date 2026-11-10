#include <iostream>
#include <limits>
#include <string>
#include "ttrianglematrix.h"

// Clears the standard input stream in case of invalid data
void clear_input() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

// Safe integer input with range validation
int read_int(const std::string& prompt, int min_val = std::numeric_limits<int>::min(), int max_val = std::numeric_limits<int>::max()) {
    int val;
    while (true) {
        std::cout << prompt;
        if (std::cin >> val && val >= min_val && val <= max_val) {
            return val;
        }
        std::cout << ">> Invalid input! Please enter an integer in range [" << min_val << " .. " << max_val << "].\n";
        clear_input();
    }
}

// Safe floating-point number input
double read_double(const std::string& prompt) {
    double val;
    while (true) {
        std::cout << prompt;
        if (std::cin >> val) {
            return val;
        }
        std::cout << ">> Invalid input! Please enter a valid number.\n";
        clear_input();
    }
}

// Input function for standard rectangular matrix
TMatrix<double> input_matrix(const std::string& name) {
    std::cout << "\n--- Input Matrix " << name << " ---\n";
    int rows = read_int("Number of rows: ", 1, 50);
    int cols = read_int("Number of columns: ", 1, 50);

    TMatrix<double> M(rows, cols);
    std::cout << "Enter matrix elements (" << rows << " x " << cols << ") row by row separated by spaces:\n";
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            std::cin >> M[i][j];
        }
    }
    std::cout << "Matrix " << name << " created successfully:\n" << M << "\n";
    return M;
}

// Input function for upper triangular matrix
TTriangleMatrix<double> input_triangle_matrix(const std::string& name) {
    std::cout << "\n--- Input Upper Triangular Matrix " << name << " ---\n";
    int dim = read_int("Matrix dimension (N x N): ", 1, 50);

    TTriangleMatrix<double> T(dim);
    std::cout << "Matrix is upper triangular. Enter only elements on and above the main diagonal:\n";
    for (int i = 0; i < dim; ++i) {
        for (int j = i; j < dim; ++j) {
            std::cout << name << "[" << i << "][" << j << "] = ";
            std::cin >> T[i][j];
        }
    }
    std::cout << "Triangular matrix " << name << " created successfully:\n" << T << "\n";
    return T;
}

// Input function for mathematical vector
TMathVector<double> input_vector(const std::string& name) {
    std::cout << "\n--- Input Vector " << name << " ---\n";
    int size = read_int("Vector dimension: ", 1, 50);

    TMathVector<double> v(size, 0);
    std::cout << "Enter " << size << " element(s) separated by spaces: ";
    for (int i = 0; i < size; ++i) {
        std::cin >> v[i];
    }
    std::cout << "Vector " << name << ": " << v << "\n";
    return v;
}

void menu_regular_matrices() {
    std::cout << "\n============================================\n";
    std::cout << "       REGULAR MATRIX OPERATIONS (TMatrix)   \n";
    std::cout << "============================================\n";

    TMatrix<double> A = input_matrix("A");
    TMatrix<double> B = input_matrix("B");

    while (true) {
        std::cout << "\nCurrent Matrices:\n";
        std::cout << "A:\n" << A << "\n\nB:\n" << B << "\n\n";
        std::cout << "1. Addition (A + B)\n";
        std::cout << "2. Subtraction (A - B)\n";
        std::cout << "3. Multiplication (A * B)\n";
        std::cout << "4. Scalar Multiplication (A * scalar)\n";
        std::cout << "5. Transpose Matrix A (A^T)\n";
        std::cout << "6. Re-enter matrices A and B\n";
        std::cout << "0. Back to Main Menu\n";

        int choice = read_int("Select an action: ", 0, 6);
        if (choice == 0) break;

        try {
            switch (choice) {
            case 1:
                std::cout << "\nRESULT (A + B):\n" << (A + B) << "\n";
                break;
            case 2:
                std::cout << "\nRESULT (A - B):\n" << (A - B) << "\n";
                break;
            case 3:
                std::cout << "\nRESULT (A * B):\n" << (A * B) << "\n";
                break;
            case 4: {
                double k = read_double("Enter scalar value: ");
                std::cout << "\nRESULT (A * " << k << "):\n" << (A * k) << "\n";
                break;
            }
            case 5:
                std::cout << "\nTRANSPOSED MATRIX A:\n" << A.transpose() << "\n";
                break;
            case 6:
                A = input_matrix("A");
                B = input_matrix("B");
                break;
            }
        }
        catch (const char* err) {
            std::cout << "\n>>> EXCEPTION: " << err << " <<<\n";
        }
        catch (const std::exception& ex) {
            std::cout << "\n>>> ERROR: " << ex.what() << " <<<\n";
        }
    }
}

void menu_triangle_matrices() {
    std::cout << "\n============================================\n";
    std::cout << "  UPPER TRIANGULAR MATRICES (TTriangleMatrix)\n";
    std::cout << "============================================\n";

    TTriangleMatrix<double> T1 = input_triangle_matrix("T1");
    TTriangleMatrix<double> T2 = input_triangle_matrix("T2");

    while (true) {
        std::cout << "\nCurrent Matrices:\n";
        std::cout << "T1:\n" << T1 << "\n\nT2:\n" << T2 << "\n\n";
        std::cout << "1. Addition (T1 + T2)\n";
        std::cout << "2. Subtraction (T1 - T2)\n";
        std::cout << "3. Multiplication (T1 * T2)\n";
        std::cout << "4. Scalar Multiplication (T1 * scalar)\n";
        std::cout << "5. Check compact memory storage (raw_row_size)\n";
        std::cout << "6. Test write below main diagonal (T1[row][col] = value)\n";
        std::cout << "7. Re-enter matrices T1 and T2\n";
        std::cout << "0. Back to Main Menu\n";

        int choice = read_int("Select an action: ", 0, 7);
        if (choice == 0) break;

        try {
            switch (choice) {
            case 1:
                std::cout << "\nRESULT (T1 + T2):\n" << (T1 + T2) << "\n";
                break;
            case 2:
                std::cout << "\nRESULT (T1 - T2):\n" << (T1 - T2) << "\n";
                break;
            case 3:
                std::cout << "\nRESULT (T1 * T2):\n" << (T1 * T2) << "\n";
                break;
            case 4: {
                double k = read_double("Enter scalar value: ");
                std::cout << "\nRESULT (T1 * " << k << "):\n" << (T1 * k) << "\n";
                break;
            }
            case 5:
                std::cout << "\nActual physical memory allocated per row in T1:\n";
                for (size_t i = 0; i < T1.rows(); ++i) {
                    std::cout << "  Row " << i << ": allocated " << T1.raw_row_size(i)
                        << " element(s) (expected: " << T1.rows() - i << ")\n";
                }
                std::cout << "-> Zeroes below the main diagonal are NOT physically allocated in memory!\n";
                break;
            case 6: {
                int r = read_int("Row index: ", 0, static_cast<int>(T1.rows()) - 1);
                int c = read_int("Column index: ", 0, static_cast<int>(T1.cols()) - 1);
                double val = read_double("New value: ");

                std::cout << "Value BEFORE write: T1[" << r << "][" << c << "] = " << T1[r][c] << "\n";
                T1[r][c] = val;
                std::cout << "Value AFTER write:  T1[" << r << "][" << c << "] = " << T1[r][c] << "\n";
                if (c < r) {
                    std::cout << "-> Cell is below the main diagonal! Write was ignored (remains 0).\n";
                }
                break;
            }
            case 7:
                T1 = input_triangle_matrix("T1");
                T2 = input_triangle_matrix("T2");
                break;
            }
        }
        catch (const char* err) {
            std::cout << "\n>>> EXCEPTION: " << err << " <<<\n";
        }
        catch (const std::exception& ex) {
            std::cout << "\n>>> ERROR: " << ex.what() << " <<<\n";
        }
    }
}

void menu_matrix_vector() {
    std::cout << "\n============================================\n";
    std::cout << "       MATRIX-VECTOR MULTIPLICATION         \n";
    std::cout << "============================================\n";
    std::cout << "1. Use regular matrix (TMatrix)\n";
    std::cout << "2. Use upper triangular matrix (TTriangleMatrix)\n";
    int type = read_int("Choose matrix type: ", 1, 2);

    try {
        if (type == 1) {
            TMatrix<double> M = input_matrix("M");
            std::cout << "For (M * v) multiplication, vector size must be " << M.cols() << ".\n";
            TMathVector<double> v = input_vector("v");
            TMathVector<double> res = M * v;
            std::cout << "\nRESULT (M * v):\n" << res << "\n";
        }
        else {
            TTriangleMatrix<double> T = input_triangle_matrix("T");
            std::cout << "For (T * v) multiplication, vector size must be " << T.cols() << ".\n";
            TMathVector<double> v = input_vector("v");
            TMathVector<double> res = T * v;
            std::cout << "\nRESULT (T * v):\n" << res << "\n";
        }
    }
    catch (const char* err) {
        std::cout << "\n>>> EXCEPTION: " << err << " <<<\n";
    }
}

void run_quick_demo() {
    std::cout << "\n============================================\n";
    std::cout << "       EXPRESS DEMONSTRATION (TASK SPEC)    \n";
    std::cout << "============================================\n";

    std::cout << "\n1. Initial upper triangular matrix A from the task:\n";
    TTriangleMatrix<int> A({
        { 3, 4, 3, 0, 0 },
        { 0, 4, 3, 5, 1 },
        { 0, 0, 3, 5, 1 },
        { 0, 0, 0, 5, 1 },
        { 0, 0, 0, 0, 1 }
        });
    std::cout << A << "\n";

    std::cout << "\n2. Modification above diagonal: A[2][3] = -6;\n";
    A[2][3] = -6;
    std::cout << "Matrix A after modification:\n" << A << "\n";

    std::cout << "\n3. Reading below diagonal:\n";
    std::cout << "A[4][1] = " << A[4][1] << " (expected 0)\n";

    std::cout << "\n4. Attempting to write below diagonal: A[4][1] = 5;\n";
    A[4][1] = 5;
    std::cout << "A[4][1] after write attempt = " << A[4][1] << " (modification ignored!)\n";

    std::cout << "\n5. Verification of compact storage (physical row capacities):\n";
    for (size_t i = 0; i < A.rows(); ++i) {
        std::cout << "  Row " << i << ": allocated size = " << A.raw_row_size(i) << " elements\n";
    }

    std::cout << "\n6. Matrix-vector multiplication:\n";
    TMathVector<int> vec({ 1, 1, 1, 1, 1 }, 0);
    std::cout << "Vector: " << vec << "\n";
    std::cout << "A * vec = " << (A * vec) << "\n";

    std::cout << "\n--------------------------------------------\n";
    std::cout << "Demonstration finished successfully!\n";
}

int main() {
    while (true) {
        std::cout << "\n============================================\n";
        std::cout << "         MATRIX CALCULATOR (C++)            \n";
        std::cout << "============================================\n";
        std::cout << "1. Regular Matrix Calculator (TMatrix)\n";
        std::cout << "2. Triangular Matrix Calculator (TTriangleMatrix)\n";
        std::cout << "3. Matrix-Vector Multiplication\n";
        std::cout << "4. Express Demonstration (Assignment Sample)\n";
        std::cout << "0. Exit\n";

        int choice = read_int("Select mode: ", 0, 4);

        if (choice == 0) {
            std::cout << "\nExiting application. Goodbye!\n";
            break;
        }

        switch (choice) {
        case 1: menu_regular_matrices(); break;
        case 2: menu_triangle_matrices(); break;
        case 3: menu_matrix_vector(); break;
        case 4: run_quick_demo(); break;
        }
    }

    return 0;
}