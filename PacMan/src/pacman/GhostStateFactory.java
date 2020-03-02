package pacman;

public class GhostStateFactory {

	public static GhostState createGhost(GhostState.State state, Ghost ghost) {
		switch(state) {
			case NORMAL:
				return new GhostNormalState(ghost);
			case PRISON:
				return new GhostPrisonState(ghost);
			case VULNERABLE:
				return new GhostVulnerableState(ghost);
			default:
				return new GhostNormalState(ghost);
			}
	}
	
}
