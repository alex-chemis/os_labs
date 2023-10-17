#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <aclapi.h>

// Function to set access control on the object
void SetAccessControl(wchar_t* objectName);

int main(int argc, char* argv[])
{
    if (argc != 2) {
        printf("Usage: %s <objectName>\n", argv[0]);
        return 1;
    }

    wchar_t objectName[256];
    mbstowcs_s(NULL, objectName, argv[1], strlen(argv[1]) + 1);

    SetAccessControl(objectName);

    for (int i = 0; i < 3; i++) {
        wchar_t commandLine[256];
        swprintf_s(commandLine, L"lab3_3A_background_program.exe %d %s", i, objectName);

        STARTUPINFO si;
        PROCESS_INFORMATION pi;
        ZeroMemory(&si, sizeof(si));
        si.cb = sizeof(si);
        ZeroMemory(&pi, sizeof(pi));

        if (CreateProcess(NULL, commandLine, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
            printf("Child process %d started\n", i);
        }
        else {
            printf("Failed to start child process %d\n", i);
        }
    }

    Sleep(5000);

    return 0;
}

void SetAccessControl(wchar_t* objectName)
{
    PACL pDACL = NULL;
    EXPLICIT_ACCESS ea;

    ZeroMemory(&ea, sizeof(EXPLICIT_ACCESS));
    PSID pSid;
    SID_IDENTIFIER_AUTHORITY auth = SECURITY_WORLD_SID_AUTHORITY;
    AllocateAndInitializeSid(&auth, 1, SECURITY_WORLD_RID, 0, 0, 0, 0, 0, 0, 0, &pSid);
    ea.grfAccessPermissions = FILE_GENERIC_READ | FILE_GENERIC_WRITE;
    ea.grfAccessMode = SET_ACCESS; // SET_ACCESS or DENY_ACCESS
    ea.grfInheritance = SUB_CONTAINERS_AND_OBJECTS_INHERIT;
    ea.Trustee.TrusteeForm = TRUSTEE_IS_SID;
    ea.Trustee.TrusteeType = TRUSTEE_IS_WELL_KNOWN_GROUP;
    ea.Trustee.ptstrName = (LPTSTR)pSid;

    SetEntriesInAcl(1, &ea, NULL, &pDACL);

    SetNamedSecurityInfo(objectName, SE_FILE_OBJECT, DACL_SECURITY_INFORMATION, NULL, NULL, pDACL, NULL);

    if (pDACL != NULL) LocalFree(pDACL);
    FreeSid(pSid);
}