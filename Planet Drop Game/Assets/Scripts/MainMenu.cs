using UnityEngine;
using UnityEngine.UI;
using UnityEngine.SceneManagement;

public class MainMenu : MonoBehaviour
{

    [SerializeField]
    private Transform rotatingStar, spaceship;

    private float shipDirection = .03f;

    [SerializeField]
    private GameObject easyModeButton;
    public bool easyMode;
    private Color easyModeSelected, easyModeDisabled;

    private void Start()
    {
        DontDestroyOnLoad(gameObject);

        easyModeSelected = new Vector4(0.5519402f, 0.7169812f, 0.5752468f,1f);
        easyModeDisabled = new Vector4(0.7264151f, 0.7264151f, 0.7264151f,1f);

    }

    private void Update()
    {
        if(rotatingStar != null)
            rotatingStar.Rotate(0f,0f,1f);

        if(spaceship != null)
        {
            spaceship.position += (Vector3.right * shipDirection);

            if (spaceship.position.x > 2.5f || spaceship.position.x < -2.5f)
                shipDirection *= -1f;
        }

    }

    public void StartGame()
    {
        SceneManager.LoadScene("Gameplay");
    }

    public void ToggleEasyMode()
    {
        easyMode = !easyMode;

        if (easyMode)
            easyModeButton.GetComponent<Image>().color = easyModeSelected;
        else
            easyModeButton.GetComponent<Image>().color = easyModeDisabled;

    }
}
