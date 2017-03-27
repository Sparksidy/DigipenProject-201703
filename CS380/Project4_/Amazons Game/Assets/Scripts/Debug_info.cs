using UnityEngine;
using UnityEngine.UI;
using System.Collections;

public class Debug_info : MonoBehaviour
{
  public Text turnCount;
  public Text MoveStat;
  public Text boardData;
  public Text currentPlayer;
  public Text AvailableLeft;
  public Text AvailableRight;

  // Use this for initialization
  void Start()
  {

  }

  // Update is called once per frame
  void Update()
  {
    AssignText(MoveStat, moveStatUpdate());
    AssignText(boardData, boardDataUpdate());
    AssignText(currentPlayer, currentPlayerUpdate());
    AssignText(turnCount, turnCountUpdate());
    AssignText(AvailableLeft, AvailableLeftUpdate());
    AssignText(AvailableRight, AvailableRightUpdate());
  }

  string moveStatUpdate()
  {
    string returnThis = "";
    switch (GameStat.moveStatus)
    {
      case MoveStatus.SelectPlayer:
        {
          returnThis = "Select Player Piece";
          break;
        }
      case MoveStatus.ValidateMove:
        {
          returnThis = "Validate Move positition";
          break;
        }
      case MoveStatus.ValidateShot:
        {
          returnThis = "Validate Shoot positition";
          break;
        }
    }

    return returnThis;
  }

  string boardDataUpdate()
  {
    string board = "";

    for (int i = 0; i < BoardData._boardSize; i++)
    {
      for (int j = 0; j < BoardData._boardSize; ++j)
      {
        board = string.Concat(board, BoardData.data[i, j]);
      }
      board = string.Concat(board, "\n");

    }

    return board;
  }

  string currentPlayerUpdate()
  {
    switch (GameStat.cp)
    {
      case CurrentPlayer.Player_Left:
        return "Black_Left";
      case CurrentPlayer.Player_Right:
        return "White_Right";
    }
    return "a";
  }

  string turnCountUpdate()
  {
    return GameStat.turnCount.ToString();
  }

  string AvailableLeftUpdate()
  {
    return GameStat.moveAvailable_L.ToString();
  }

  string AvailableRightUpdate()
  {
    return GameStat.moveAvailable_R.ToString();
  }
  //Helper
  void AssignText(Text t, string s)
  {
    if(t!=null)
    t.text = s;
  }
}
