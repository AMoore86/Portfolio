using UnityEngine;
using UnityEngine.UI;
using UnityEngine.SceneManagement;

public class MainMenu : MonoBehaviour
{

    [SerializeField]
    private Transform rotatingStar, spaceship;

    private float shipDirection = .03f;

    private void Update()
    {
        rotatingStar.Rotate(0f,0f,1f);

        spaceship.position += (Vector3.right * shipDirection);

        if (spaceship.position.x > 2.5f || spaceship.position.x < -2.5f)
            shipDirection *= -1f;

    }

    public void StartGame()
    {
        SceneManager.LoadScene("Gameplay");
    }

    public void Options()
    {
        //Potential Future Options Menu
    }
}
