#include "headers.h"

static void
text_mode (void)
{
    EFI_GUID console_guid = EFI_CONSOLE_CONTROL_PROTOCOL_GUID;
    EFI_CONSOLE_CONTROL_PROTOCOL *cons;
    EFI_CONSOLE_CONTROL_SCREEN_MODE mode;

    if (EFI_SUCCESS != LibLocateProtocol (&console_guid, (VOID **) & cons))
        return;

    uefi_call_wrapper (cons->GetMode, 4, cons, &mode, NULL, NULL);
    uefi_call_wrapper (cons->SetMode, 2, cons, EfiConsoleControlScreenText);
}

EFI_STATUS EFIAPI
efi_main (EFI_HANDLE image_handle, EFI_SYSTEM_TABLE *system_table)
{
    InitializeLib (image_handle, system_table);

    text_mode();

    Print (L"Hello world\n");

    uefi_call_wrapper (system_table->BootServices->Stall, 1, 10000);

    return EFI_SUCCESS;
}
