namespace Bolun.Phone.Sdk;

public enum BolunApplicationCapability
{
    Network,
    Storage,
    Notifications,
}

public sealed class BolunApplicationManifest
{
    public required string ApplicationId { get; init; }
    public required string DisplayName { get; init; }
    public IReadOnlyList<BolunApplicationCapability> Capabilities { get; init; } = Array.Empty<BolunApplicationCapability>();
}
