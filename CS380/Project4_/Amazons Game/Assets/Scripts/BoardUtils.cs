using UnityEngine;
using System.Collections;

// Put this script on a Camera
public class BoardUtils : MonoBehaviour
{
  // Fill/drag these in from the editor

  // Choose the Unlit/Color shader in the Material Settings
  // You can change that color, to change the color of the connecting lines
  public Material lineMat;
  public Material shotMat;
  public Material validMat;

  static public Material _shotMat;
  static public Material _validMat;
  static public float cellSize = 1;

  void Start()
  {
    _shotMat = shotMat;
    _validMat = validMat;
  }


  void DrawLine(Vector2 begin, Vector2 end)
  {
    GL.Begin(GL.LINES);
    lineMat.SetPass(0);
    GL.Color(new Color(lineMat.color.r, lineMat.color.g, lineMat.color.b, lineMat.color.a));
    GL.Vertex3(begin.x, begin.y, 0 );
    GL.Vertex3(end.x, end.y, 0);
    GL.End();
  }

  static public void DrawQuad(int row, int col)
  {

    float size = cellSize * 0.5f;
    Vector3 max = new Vector3(size / 2, size / 2, 0) + GetCoordinate(row, col);
    Vector3 min = new Vector3(-size / 2, -size / 2, 0) + GetCoordinate(row, col);


    GL.Begin(GL.QUADS);
    _validMat.SetPass(0);

    GL.Vertex3(min.x, min.y, 0);
    GL.Vertex3(min.x, max.y, 0);
    GL.Vertex3(max.x, max.y, 0);
    GL.Vertex3(max.x, min.y, 0);
    GL.End();
  }

  static public void DrawCross(int row, int col)
  {
    float size = cellSize * 0.5f;
    Vector3 max = new Vector3(size / 2, size / 2,0) + GetCoordinate(row,col);
    Vector3 min = new Vector3(-size / 2, -size / 2,0) + GetCoordinate(row, col);


    GL.Begin(GL.LINES);
    _shotMat.SetPass(0);
    GL.Vertex3(min.x, min.y, 0);
    GL.Vertex3(max.x, max.y, 0);

    GL.Vertex3(min.x, max.y, 0);
    GL.Vertex3(max.x, min.y, 0);
    GL.End();

  }

  void DrawingBoard()
  {
    float LineLength = cellSize * BoardData._boardSize;
    Vector2[] horizontal = new Vector2[] 
    {
      new Vector2( LineLength / 2, 0),
      new Vector2(-LineLength / 2, 0)
    };
    Vector2[] vertical = new Vector2[] 
    {
      new Vector2(0,  LineLength / 2),
      new Vector2(0, -LineLength / 2)
    };

    DrawLine(horizontal[0], horizontal[1]);
    DrawLine(vertical[0], vertical[1]);


    for (int i = 1; i < BoardData._boardSize / 2 + 1; ++i)
    {
      Vector2 horInc = new Vector2(0, cellSize);
      Vector2 verInc = new Vector2(cellSize, 0);

      DrawLine(horizontal[0]  + horInc * i, horizontal[1] + horInc * i);
      DrawLine(horizontal[0] - horInc * i, horizontal[1] - horInc * i);

      DrawLine(vertical[0] + verInc * i, vertical[1] + verInc * i);
      DrawLine(vertical[0] - verInc * i, vertical[1] - verInc * i);

    }

  }

  // To show the lines in the game window whne it is running
  void OnPostRender()
  {
    DrawingBoard();
  }

  static public Vector3 GetCoordinate(int row, int col)
  {
    float x = -(cellSize) * BoardData._boardSize /2 + cellSize/2;
    float y = (cellSize) * BoardData._boardSize / 2 - cellSize / 2;
    return new Vector3(col * cellSize + x, y - row * cellSize, 0);
  }

  static public gridPos GetPosition(Vector3 pos)
  {
    float x0 = -(cellSize) * BoardData._boardSize / 2 ;
    float y0 = (cellSize) * BoardData._boardSize / 2;
    float x1 = pos.x;
    float y1 = pos.y;

    float xdif = x1 - x0;
    float ydif = -(y1 - y0);


    gridPos returnThis = new gridPos();
    returnThis.x = (int)(xdif/cellSize);
    returnThis.y = (int)(ydif / cellSize);
    return returnThis;
  }

  static public Vector3 GetMousePosition()
  {
    Ray ray = Camera.main.ScreenPointToRay(Input.mousePosition);
    RaycastHit hit;
    if (Physics.Raycast(ray, out hit))
    {
      if (hit.transform.gameObject.tag == "Board")
      {
        return hit.point;
      }
    }

    return new Vector3(0, 0, 0);
  }
}