using UnityEngine;
using System.Collections.Generic;
using System.Collections;

public enum MoveStatus
{
  SelectPlayer = 0,
  ValidateMove = 1,
  ValidateShot = 2
};

public enum CurrentPlayer
{
  Player_Left = 1,
  Player_Right = 2
}

public class GameStat
{
  public static int turnCount = 0;
  public static CurrentPlayer cp = CurrentPlayer.Player_Left;
  public static MoveStatus moveStatus = MoveStatus.SelectPlayer;
  public static int moveAvailable_L = 100;
  public static int moveAvailable_R = 100;

  public static void NextMoveStat()
  {
    if (moveStatus == MoveStatus.ValidateShot)
    {
      moveStatus = MoveStatus.SelectPlayer;
      return;
    }

    moveStatus++;
  }
}

public class Move
{
  public gridPos originalPos;
  public gridPos moveTo;
  public gridPos shotTo;
}

public class GameController : MonoBehaviour
{
  public gridPos clickedPos = new gridPos(-1, -1);
  List<gridPos> validateMove = new List<gridPos>();
  List<gridPos> validateShot = new List<gridPos>();
  Move currentMove = new Move();

  // Use this for initialization
  void Start()
  {

  }

  // Update is called once per frame
  void Update()
  {
    if (GameStat.moveAvailable_L == 0 || GameStat.moveAvailable_R == 0)
    {
      GetComponent<BoardData>().RestartGame();
      Debug.Log("Game Restarted");
      return;
    }

    UpdateCP();

    bool isAI = false;
    switch (GameStat.cp)
    {
      case (CurrentPlayer)1:
        {
          isAI = BoardData.P1AI;
          break;
        }
      case (CurrentPlayer)2:
        {
          isAI = BoardData.P2AI;
          break;
        }
    }

    if (!isAI)
      PlayerInput();
    else
      AI_Random();

  }

  void UpdateCP()
  {
    int cp = GameStat.turnCount % 2;
    if (cp == 0)
      GameStat.cp = CurrentPlayer.Player_Left;
    else
      GameStat.cp = CurrentPlayer.Player_Right;
  }

  void AI_Random()
  {
    UpdateCP();
    System.Random rand = new System.Random();

    //Decide current player(Left or Right)
    GameObject[] currentPlayer = new GameObject[BoardData.playerCount];
    switch (GameStat.cp)
    {
      case (CurrentPlayer)1:
        {
          currentPlayer = Player.lefts;
          break;
        }
      case (CurrentPlayer)2:
        {
          currentPlayer = Player.rights;
          break;
        }
    }

    //select piece

    int playerPiece = rand.Next(0, 3);
    GameObject selectedPience = currentPlayer[playerPiece];
    pPosition piecePos = selectedPience.GetComponent<pPosition>();
    gridPos pos = piecePos.pos;

    DecideMove(pos);
    if (validateMove.Count <= 0)
      for (int i = 0; i < 4; ++i)
      {
        Reset();
        selectedPience = currentPlayer[i];
        piecePos = selectedPience.GetComponent<pPosition>();
        pos = piecePos.pos;

        DecideMove(pos);
        if (validateMove.Count > 0)
        {
          break;
        }
      }

    Debug.Log("AI decided piece to move");

    //select move
    int moveSelectedId = rand.Next(0, validateMove.Count - 1);
    DecideMove(validateMove[moveSelectedId]);
    Debug.Log("AI decided move position");

    //select shot
    int shotSelectedId = rand.Next(0, validateShot.Count - 1);
    DecideMove(validateShot[shotSelectedId]);
    Debug.Log("AI decided shot position");

  }

  void PlayerInput()
  {
    if (Input.GetMouseButtonDown(1))
    {
      Reset();
    }

    if (Input.GetMouseButtonDown(0))
    {
      clickedPos = BoardUtils.GetPosition(BoardUtils.GetMousePosition());
      DecideMove(clickedPos);
    }

  }

  void OnPostRender()
  {
    foreach (gridPos t in validateMove)
    {
      BoardUtils.DrawQuad(t.y, t.x);
    }

    foreach (gridPos t in validateShot)
    {
      BoardUtils.DrawQuad(t.y, t.x);
    }
  }
  //////////////////////////////////////////////////////////////////////////////////////////
  ///////////////////////////////////////LOGIC/////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////
  List<gridPos> ValidateMove(int x, int y)
  {
    List<gridPos> returnThis = new List<gridPos>();
    //check horizontal
    int curX = x;
    int curY = y;

    while (curY < BoardData._boardSize - 1)
    {
      curY++;

      if (BoardData.data[curY, curX] != 0)
        break;
      returnThis.Add(new gridPos(curX, curY));
    }

    curY = y;
    while (curY > 0)
    {
      curY--;

      if (BoardData.data[curY, curX] != 0)
        break;
      returnThis.Add(new gridPos(curX, curY));
    }
    //check vertical
    curY = y;
    curX = x;
    while (curX < BoardData._boardSize - 1)
    {
      curX++;

      if (BoardData.data[curY, curX] != 0)
        break;
      returnThis.Add(new gridPos(curX, curY));
    }

    curY = y;
    curX = x;
    while (curX > 0)
    {
      curX--;

      if (BoardData.data[curY, curX] != 0)
        break;
      returnThis.Add(new gridPos(curX, curY));
    }

    //check diagonal
    curY = y;
    curX = x;
    while (curX < BoardData._boardSize - 1 && curY > 0)
    {
      curX++;
      curY--;
      if (BoardData.data[curY, curX] != 0)
        break;
      returnThis.Add(new gridPos(curX, curY));
    }

    curY = y;
    curX = x;
    while (curY < BoardData._boardSize - 1 && curX > 0)
    {
      curX--;
      curY++;
      if (BoardData.data[curY, curX] != 0)
        break;
      returnThis.Add(new gridPos(curX, curY));
    }

    curY = y;
    curX = x;
    while (curY > 0 && curX > 0)
    {
      curX--;
      curY--;
      if (BoardData.data[curY, curX] != 0)
        break;
      returnThis.Add(new gridPos(curX, curY));
    }

    curY = y;
    curX = x;
    while (curY < BoardData._boardSize - 1 && curX < BoardData._boardSize - 1)
    {
      curX++;
      curY++;
      if (BoardData.data[curY, curX] != 0)
        break;
      returnThis.Add(new gridPos(curX, curY));
    }

    //draw feedback
    return returnThis;
  }

  List<gridPos> ValidateShot(int x, int y)
  {
    List<gridPos> returnThis = new List<gridPos>();
    //check horizontal
    int curX = x;
    int curY = y;

    while (curY < BoardData._boardSize - 1)
    {
      curY++;

      if (BoardData.data[curY, curX] != 0 && !(new gridPos(curX, curY).Equals(currentMove.originalPos)))
        break;
      returnThis.Add(new gridPos(curX, curY));
    }

    curY = y;
    while (curY > 0)
    {
      curY--;

      if (BoardData.data[curY, curX] != 0 && !(new gridPos(curX, curY).Equals(currentMove.originalPos)))
        break;
      returnThis.Add(new gridPos(curX, curY));
    }
    //check vertical
    curY = y;
    curX = x;
    while (curX < BoardData._boardSize - 1)
    {
      curX++;
      bool isOriginalPos = (new gridPos(curX, curY)).Equals(currentMove.originalPos);
      if (BoardData.data[curY, curX] != 0 && !isOriginalPos)
        break;
      returnThis.Add(new gridPos(curX, curY));
    }

    curY = y;
    curX = x;
    while (curX > 0)
    {
      curX--;

      if (BoardData.data[curY, curX] != 0 && !(new gridPos(curX, curY).Equals(currentMove.originalPos)))
        break;
      returnThis.Add(new gridPos(curX, curY));
    }

    //check diagonal
    curY = y;
    curX = x;
    while (curX < BoardData._boardSize - 1 && curY > 0)
    {
      curX++;
      curY--;
      if (BoardData.data[curY, curX] != 0 && !(new gridPos(curX, curY).Equals(currentMove.originalPos)))
        break;
      returnThis.Add(new gridPos(curX, curY));
    }

    curY = y;
    curX = x;
    while (curY < BoardData._boardSize - 1 && curX > 0)
    {
      curX--;
      curY++;
      if (BoardData.data[curY, curX] != 0 && !(new gridPos(curX, curY).Equals(currentMove.originalPos)))
        break;
      returnThis.Add(new gridPos(curX, curY));
    }

    curY = y;
    curX = x;
    while (curY > 0 && curX > 0)
    {
      curX--;
      curY--;
      if (BoardData.data[curY, curX] != 0 && !(new gridPos(curX, curY).Equals(currentMove.originalPos)))
        break;
      returnThis.Add(new gridPos(curX, curY));
    }

    curY = y;
    curX = x;
    while (curY < BoardData._boardSize - 1 && curX < BoardData._boardSize - 1)
    {
      curX++;
      curY++;
      if (BoardData.data[curY, curX] != 0 && !(new gridPos(curX, curY).Equals(currentMove.originalPos)))
        break;
      returnThis.Add(new gridPos(curX, curY));
    }

    //draw feedback
    return returnThis;
  }

  void CalculateAvailableMove()
  {
    Debug.Log("Calculating Available Move");

    GameStat.moveAvailable_L = 0;
    GameStat.moveAvailable_R = 0;
    for (int i = 0; i < BoardData.playerCount; ++i)
    {
      GameObject leftPiece = Player.lefts[i];
      GameObject rightPiece = Player.rights[i];

      gridPos checkLeft = leftPiece.GetComponent<pPosition>().pos;
      gridPos checkRight = rightPiece.GetComponent<pPosition>().pos;

      int curPiece_L = ValidateMove(checkLeft.x, checkLeft.y).Count;
      int curPiece_R = ValidateMove(checkRight.x, checkRight.y).Count;
      //Debug.Log("checking piece (x:" + leftPiece.GetComponent<pPosition>().pos.x + ",y:" + leftPiece.GetComponent<pPosition>().pos.y + ") = " + curPiece_L);
      //Debug.Log("checking piece (x:" + rightPiece.GetComponent<pPosition>().pos.x + ",y:" + rightPiece.GetComponent<pPosition>().pos.y + ") = " + curPiece_R);

      GameStat.moveAvailable_L += curPiece_L;
      GameStat.moveAvailable_R += curPiece_R;
    }
    Debug.Log("AvailableMove Calculated");
  }

  void ExecuteMove(Move m)
  {
    int curPlayer = BoardData.data[m.originalPos.y, m.originalPos.x];
    BoardData.data[m.originalPos.y, m.originalPos.x] = 0;
    BoardData.data[m.moveTo.y, m.moveTo.x] = curPlayer;
    BoardData.data[m.shotTo.y, m.shotTo.x] = 3;

  }

  //////////////////////////////////////////////////////////////////////////////////////////
  ///////////////////////////////////////HELPER/////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////
  bool FindInList(List<gridPos> l, gridPos a)
  {
    foreach (gridPos it in l)
    {
      if (it.Equals(a))
        return true;
    }

    return false;
  }

  void DecideMove(gridPos clickedPos)
  {
    switch (GameStat.moveStatus)
    {
      case MoveStatus.SelectPlayer:
        {
          SelectPlayer(clickedPos);
          break;
        }

      case MoveStatus.ValidateMove:
        {
          SelectMove(clickedPos);
          break;
        }

      case MoveStatus.ValidateShot:
        {
          SelectShot(clickedPos);
          break;
        }
    }
  }

  void SelectPlayer(gridPos clickedPos)
  {
    if (BoardData.data[clickedPos.y, clickedPos.x] == (int)GameStat.cp)
    {
      validateMove = ValidateMove(clickedPos.x, clickedPos.y);
      currentMove.originalPos = clickedPos;
      GameStat.NextMoveStat();
    }
  }
  void SelectMove(gridPos clickedPos)
  {
    if (FindInList(validateMove, clickedPos))
    {
      validateMove.Clear();
      validateShot = ValidateShot(clickedPos.x, clickedPos.y);
      currentMove.moveTo = clickedPos;
      GameStat.NextMoveStat();
    }
  }
  void SelectShot(gridPos clickedPos)
  {
    if (FindInList(validateShot, clickedPos))
    {
      validateShot.Clear();
      currentMove.shotTo = clickedPos;
      GameStat.turnCount++;
      ExecuteMove(currentMove);
      GameStat.NextMoveStat();
      CalculateAvailableMove();
    }
  }
  void Reset()
  {
    GameStat.moveStatus = MoveStatus.SelectPlayer;
    validateMove.Clear();
    validateShot.Clear();
  }
}
