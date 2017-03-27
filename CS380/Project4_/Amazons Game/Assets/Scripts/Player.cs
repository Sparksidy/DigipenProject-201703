using UnityEngine;
using System.Collections;

public class Player : MonoBehaviour
{
  public GameObject left;
  public GameObject right;

  public static GameObject[] lefts;
  public static GameObject[] rights;
  // Use this for initialization
  void Start()
  {
    lefts = new GameObject[BoardData.playerCount];
    rights = new GameObject[BoardData.playerCount];

    for (int i = 0; i < BoardData.playerCount; ++i)
    {
      lefts[i] = GameObject.Instantiate(left);
      rights[i] = GameObject.Instantiate(right);
    }
  }

  // Update is called once per frame
  void Update()
  {
    SetBoard();
  }

  void OnPostRender()
  {
    SetBoard();
  }

  void SetBoard()
  {
    int leftFound = 0;
    int rightFound = 0;

    for (int r = 0; r < BoardData._boardSize; ++r)
    {
      for (int c = 0; c < BoardData._boardSize; ++c)
      {
        if (BoardData.data[r, c] == 1)
        {
          lefts[leftFound].transform.position = BoardUtils.GetCoordinate(r, c);
          lefts[leftFound].GetComponent<pPosition>().pos.x = c;
          lefts[leftFound].GetComponent<pPosition>().pos.y = r;
          leftFound++;
        }
        if (BoardData.data[r, c] == 2)
        {
          rights[rightFound].transform.position = BoardUtils.GetCoordinate(r, c);
          rights[rightFound].GetComponent<pPosition>().pos.x = c;
          rights[rightFound].GetComponent<pPosition>().pos.y = r;
          rightFound++;
        }
        if (BoardData.data[r, c] == 3)
        {
          BoardUtils.DrawCross(r, c);
        }
      }
    }//endFor
  }//SetBoard
}
