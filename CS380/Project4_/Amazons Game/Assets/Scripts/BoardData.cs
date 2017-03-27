using UnityEngine;
using System.Collections;
using UnityEngine.UI;

public class gridPos
{
  public gridPos() { x = 0; y = 0; }
  public gridPos(gridPos g) { x = g.x; y = g.y; }
  public gridPos(int _x, int _y) { x = _x; y = _y; }
  public int x, y;
  public bool Equals(gridPos g) { return x == g.x && y == g.y; }
}

public class BoardData : MonoBehaviour
{
  public int boardSize = 10;
  public Toggle p1AI;
  public Toggle p2AI;

  static public int _boardSize;
  static public int[,] data;
  static public bool P1AI = false;
  static public bool P2AI = false;
  static public int playerCount = 4;
  // Use this for initialization
  void Start()
  {
    data = new int[10,10];

    if (boardSize % 2 != 0)
      boardSize += 1;
    _boardSize = boardSize;

    RestartGame();
  }

  void Update()
  {

  }

  // Update is called once per frame
  public void RestartGame()
  {
    //initializeBoard
    for (int r = 0; r < _boardSize; ++r)
    {
      for (int c = 0; c < _boardSize; ++c)
      {
        data[r, c] = 0;
      }
    }

    data[0, 3] = 1;
    data[0, 6] = 1;
    data[3, 0] = 1;
    data[3, 9] = 1;

    data[6, 0] = 2;
    data[6, 9] = 2;
    data[9, 3] = 2;
    data[9, 6] = 2;

    P1AI = p1AI.isOn;
    P2AI = p2AI.isOn;

    GameStat.turnCount = 0;
    GameStat.moveAvailable_L = 100;
    GameStat.moveAvailable_R = 100;

  }

}
