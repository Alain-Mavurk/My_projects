package pacman;

public class PacdotFactory {
	public static Pacdot createPacdot(Game game, Coordinate position, int type) {
		switch(type) {
			case Maze.NORMAL_PACDOT:
				return new PacdotNormal(game, position);
			case Maze.MAZE_PACDOT:
				return new PacdotMaze(game, position);
			case Maze.INVISIBLE_PACDOT:
				return new PacdotInvisible(game, position);
			case Maze.SUPER_PACDOT:
				return new PacdotSuper(game, position);
			default :
				return new PacdotNormal(game, position);
		}
	}
}
