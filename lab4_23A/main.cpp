#include <windows.h>
#include <stdio.h>

int main() {
    STARTUPINFO siWriter, siReader;
    PROCESS_INFORMATION piWriter, piReader;

    // Initialize the startup structures
    ZeroMemory(&siWriter, sizeof(siWriter));
    ZeroMemory(&siReader, sizeof(siReader));
    siWriter.cb = sizeof(siWriter);
    siReader.cb = sizeof(siReader);

    wchar_t writer[256];
    swprintf_s(writer, L"lab4_23A_writer.exe");
    wchar_t reader[256];
    swprintf_s(reader, L"lab4_23A_reader.exe");

    // Create the writer process
    if (!CreateProcess(NULL, writer, NULL, NULL, FALSE, 0, NULL, NULL, &siWriter, &piWriter)) {
        printf("Failed to create writer process. Error: %d\n", GetLastError());
        return 1;
    }

    // Create the reader process
    if (!CreateProcess(NULL, reader, NULL, NULL, FALSE, 0, NULL, NULL, &siReader, &piReader)) {
        printf("Failed to create reader process. Error: %d\n", GetLastError());
        return 1;
    }

    // Wait for the processes to finish
    WaitForSingleObject(piWriter.hProcess, INFINITE);
    WaitForSingleObject(piReader.hProcess, INFINITE);

    // Close process and thread handles
    CloseHandle(piWriter.hProcess);
    CloseHandle(piWriter.hThread);
    CloseHandle(piReader.hProcess);
    CloseHandle(piReader.hThread);

    return 0;
}