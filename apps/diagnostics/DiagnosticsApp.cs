namespace Bolun.Phone.Apps.Diagnostics;

public sealed record DiagnosticItem(string Name, bool Passed, string Detail);

public static class DiagnosticsApp
{
    public static DiagnosticItem CreateBootDiagnostic(bool bootInfoValid) =>
        new("Boot ABI", bootInfoValid, bootInfoValid ? "Boot metadata accepted" : "Boot metadata rejected");
}
