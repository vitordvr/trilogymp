#include "StdInc.h"

BOOL StartGtaProcess()
{
    LPCWSTR lpApplicationName = L"E:\\Games\\GTA San Andreas - Definitive Edition\\Gameface\\Binaries\\Win64\\SanAndreas.exe";
    LPWSTR lpCommandLine = L"";

    STARTUPINFOW si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);

    if (CreateProcessW(
        lpApplicationName,   // Caminho para o executável
        lpCommandLine,       // Comando a ser passado para o processo
        NULL,                // Atributos de segurança do processo (padrão)
        NULL,                // Atributos de segurança do thread (padrão)
        FALSE,               // Herança de identificadores do processo e thread (não herdar)
        0,                   // Flags de criação (padrão)
        NULL,                // Variáveis de ambiente do processo pai (herdar)
        NULL,                // Diretório inicial do processo (padrão)
        &si,                 // Informações de inicialização do processo
        &pi                  // Informações sobre o processo criado
    ))
    {
        // Aguarda o término do processo filho
        WaitForSingleObject(pi.hProcess, INFINITE);

        // Fecha os identificadores do processo e do thread
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);

        return 0;
    }
    else
    {
        // Se CreateProcessW falhar, imprime uma mensagem de erro
        wprintf(L"Erro %d ao criar o processo: %s\n", GetLastError(), lpApplicationName);
        return 1;
    }
}

int LaunchGame() {
    StartGtaProcess();
    return 0;
}


