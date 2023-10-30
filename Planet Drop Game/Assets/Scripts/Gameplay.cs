using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using TMPro;
using System.IO;
using UnityEngine.SceneManagement;

public class Gameplay : MonoBehaviour
{
    #region Inspecter Values

    [Header("Dropper")]
    [SerializeField]
    private Transform dropper;
    [SerializeField]
    private Transform dropPoint;
    [SerializeField]
    private float moveSpeed = 3f;
    [SerializeField]
    private GameObject guideline;
    [SerializeField]
    private Transform sensor, staticSensor;
    
    [Header("Drops")]
    public List<GameObject> drops = new List<GameObject>();
    [SerializeField]
    private Queue<GameObject> upcomingDrops = new Queue<GameObject>();
    public Transform dropsSpawned;

    [Header("HUD")]
    [SerializeField]
    private GameObject HUD;
    [SerializeField]
    private Transform scoreBubble, highscoreBubble, nextDropBubble, dropOrderBubble;
    [SerializeField]
    private TMP_Text currentScoreText, highScoreText, nextDropTitle, dropOrderListTitle, dropOrderList;
    [SerializeField]
    private Image nextDropImage;
    public int currentScore;
    public int highScore;

    [Header("End Modal HUD")]
    [SerializeField]
    private GameObject EndHUD;
    [SerializeField]
    private Transform endContainer;
    [SerializeField]
    private TMP_Text endScoreText;
    [SerializeField]
    private GameObject retryButton, menuButton, quitButton, muteButton;
    private bool muted;
    [SerializeField]
    private Sprite soundIcon, muteIcon;

    #endregion

    //Keep track of the object currently ready to drop or dropping
    private GameObject currentDrop;
    private float currentRadius;

    private RaycastHit2D ray;

    [HideInInspector]
    public bool endGame;

    #region Audio
    [Header("Audio")]
    [SerializeField]
    private AudioSource audio_Pop;
    [SerializeField]
    private AudioSource audio_Rock;

    #endregion

    // Start is called before the first frame update
    void Start()
    {
        //Initialize the HUD
        HUD.SetActive(true);
        EndHUD.SetActive(false);


        //Initialize drop queue for start of game
        for(int i = 0; i < 3; i++)
            upcomingDrops.Enqueue(drops[Random.Range(0, 5)]);

        //Pull the next drop from the queue and hold it at dropper's location
        currentDrop = Instantiate(upcomingDrops.Dequeue(), dropPoint.position + new Vector3(Random.Range(-0.001f, 0.001f), 0, 0), Quaternion.identity, dropper);
        currentDrop.GetComponent<Rigidbody2D>().gravityScale = 0;
        currentRadius = currentDrop.transform.localScale.x / 2f;

        //Set the sprite and color of the next drop accordingly
        nextDropImage.sprite = upcomingDrops.Peek().transform.GetChild(0).GetComponent<SpriteRenderer>().sprite;
        nextDropImage.color = upcomingDrops.Peek().transform.GetChild(0).GetComponent<SpriteRenderer>().color;

        //Get current saved high score
        highScore = GetHighScore();
        highScoreText.text = "Highscore\n" + highScore;

        DynamicHUD(Screen.width, Screen.height);
    }

    // Update is called once per frame
    void Update()
    {
        //Stop the ability to play if the game has ended
        if (endGame)
            return;

        //Raycast to check if dropper needs to move upward
        ray = Physics2D.Raycast(sensor.position, Vector2.right, 40f);
        if (ray.collider != null)
            Elevator(ray.collider, 1f);

        //Raycast to check if dropper needs to move downward
        ray = Physics2D.Raycast(staticSensor.position, Vector2.right, 40f);
        if (ray.collider == null && dropper.position.y > 4f)
            Elevator(ray.collider, -1f);

        //Force dropper within container boundaries
        if (dropper.position.x <= -4.90f + currentRadius)
            dropper.position += Vector3.right * 0.05f;
        if(dropper.position.x >= 4.90f - currentRadius)
            dropper.position += Vector3.left * 0.05f;

        //Move the dropper left
        if ((Input.GetKey(KeyCode.A) || Input.GetKey(KeyCode.LeftArrow)) 
            && dropper.position.x > -4.90f + currentRadius)
            dropper.position += Vector3.left * moveSpeed * Time.deltaTime;

        //Move the dropper right
        else if((Input.GetKey(KeyCode.D) || Input.GetKey(KeyCode.RightArrow)) 
            && dropper.position.x < 4.90f - currentRadius)
            dropper.position += Vector3.right * moveSpeed * Time.deltaTime;

        //Drop whatever the dropper is currently holding
        if (Input.GetKeyDown(KeyCode.Space) && upcomingDrops.Count == 2)
            Drop();

        //Toggle pause modal
        if (Input.GetKeyDown(KeyCode.Escape))
            TogglePause();

        //Update Score Text
        UpdateCurrentScore();

        //Load the next drop into the dropper
        if (currentDrop == null || currentDrop.GetComponent<Drop>().hitSomething)
            LoadNextDrop();

    }

    #region Game Mechanics
    private void Drop()
    {
        if (currentDrop == null)
            return;

        if(guideline.activeSelf && !muted)
            audio_Pop.Play();

        //Allow current drop to fall and put its gameobject into a bin of all spawned drops
        currentDrop.transform.SetParent(dropsSpawned);
        currentDrop.GetComponent<Rigidbody2D>().gravityScale = 3;
        currentDrop.layer = 7;

        //Hide guidline while dropping
        guideline.SetActive(false);


    }

    public void LoadNextDrop()
    {
        if(currentDrop != null)
            currentDrop.GetComponent<Rigidbody2D>().gravityScale = 1;

        //Insert random drop into queue
        upcomingDrops.Enqueue(drops[Random.Range(0, 5)]);

        //Pull the next drop from the queue and hold it at dropper's location
        currentDrop = Instantiate(upcomingDrops.Dequeue(), dropPoint.position + new Vector3(Random.Range(-0.001f,0.001f),0,0), Quaternion.identity, dropper);
        currentDrop.GetComponent<Rigidbody2D>().gravityScale = 0;
        currentRadius = currentDrop.transform.localScale.x / 2f;

        //Set the sprite and color of the next drop accordingly
        nextDropImage.sprite = upcomingDrops.Peek().transform.GetChild(0).GetComponent<SpriteRenderer>().sprite;
        nextDropImage.color = upcomingDrops.Peek().transform.GetChild(0).GetComponent<SpriteRenderer>().color;

        //Turn guidline on for next drop
        guideline.SetActive(true);

    }

    private int GetHighScore()
    {
        int hs = 0;

        //Retrieving a highscore if one is saved locally
        if (File.Exists("highscore.txt"))
        {
            StreamReader reader = new StreamReader("highscore.txt");

            hs = System.Int32.Parse(reader.ReadLine());

            reader.Close();
        }

        return hs;
    }

    private void UpdateCurrentScore()
    {
        //Reflect HUD text with current score
        currentScoreText.text = "Score\n" + currentScore;
        endScoreText.text = "Score: " + currentScore;
    }

    private void Elevator(Collider2D collider, float elevateFactor)
    {
        if(collider != null)
        {
            GameObject drop = collider.gameObject;

            //Do not modify dropper's elevation if the object is a falling (new) drop
            if (drop.layer == 7 && !drop.GetComponent<Drop>().hitSomething)
                return;
        }

        //Slowly modify dropper's elevation as necessary
        dropper.position += Vector3.up * 0.01f * elevateFactor;
        transform.position += Vector3.up * 0.01f * elevateFactor;

        //Zoom camera according to height of dropper
        Camera.main.orthographicSize += 0.01f * elevateFactor;
    }

    private void TogglePause()
    {
        //Toggle the pause/end modal
        EndHUD.SetActive(!EndHUD.activeSelf);

        //Toggle game time as necessarry
        if (EndHUD.activeSelf)
            Time.timeScale = 0f;
        else
            Time.timeScale = 1f;
    }

    public void ToggleMute()
    {
        muted = !muted;

        if (muted)
            muteButton.GetComponent<Image>().sprite = muteIcon;
        else
            muteButton.GetComponent<Image>().sprite = soundIcon;
    }
    #endregion

    #region Dynamic HUD
    private void DynamicHUD(int width, int height)
    {
        
        //Normal HUD Variables
        Vector2 scoreBubbleSize = new Vector2(250, 150);
        Vector2 nextDropBubbleSize = new Vector2(200, 200);
        Vector2 dropOrderBubbleSize = new Vector2(250, 400);
        Vector2 nextDropImageSize = new Vector2(100, 100);
        int mainFontSize = 36;
        int listFontSize = 32;

        //EndHUD Variables
        endContainer.GetComponent<RectTransform>().sizeDelta = new Vector2(width / 2, height / 2);
        Vector2 buttonSize = new Vector2(160, 30);
        int buttonTextFontSize = 24;

        #region Breakpoints
        if (width > 3000)
        {
            scoreBubbleSize = new Vector2(500, 300);
            nextDropBubbleSize = new Vector2(400, 400);
            dropOrderBubbleSize = new Vector2(500, 800);
            nextDropImageSize = new Vector2(200, 200);
            mainFontSize = 60;
            listFontSize = 50;

            buttonSize = new Vector2(320, 60);
            buttonTextFontSize = 40;
        }
        else if(width < 1400)
        {
            scoreBubbleSize = new Vector2(175, 100);
            nextDropBubbleSize = new Vector2(150, 150);
            dropOrderBubbleSize = new Vector2(175, 275);
            nextDropImageSize = new Vector2(75, 75);
            mainFontSize = 25;
            listFontSize = 22;
        }
        #endregion

        ///Setting component parameters
        //Bubble Sizes
        scoreBubble.GetComponent<RectTransform>().sizeDelta = scoreBubbleSize;
        highscoreBubble.GetComponent<RectTransform>().sizeDelta = scoreBubbleSize;
        nextDropBubble.GetComponent<RectTransform>().sizeDelta = nextDropBubbleSize;
        dropOrderBubble.GetComponent<RectTransform>().sizeDelta = dropOrderBubbleSize;

        //Bubble Info Sizes
        currentScoreText.fontSize = mainFontSize;
        highScoreText.fontSize = mainFontSize;
        nextDropImage.GetComponent<RectTransform>().sizeDelta = nextDropImageSize;
        nextDropTitle.fontSize = mainFontSize;
        dropOrderListTitle.fontSize = mainFontSize;
        dropOrderList.fontSize = listFontSize;

        //EndHUD
        endScoreText.fontSize = mainFontSize;

        retryButton.GetComponent<RectTransform>().sizeDelta = buttonSize;
        menuButton.GetComponent<RectTransform>().sizeDelta = buttonSize;
        quitButton.GetComponent<RectTransform>().sizeDelta = buttonSize;

        retryButton.GetComponentInChildren<TMP_Text>().fontSize = buttonTextFontSize;
        menuButton.GetComponentInChildren<TMP_Text>().fontSize = buttonTextFontSize;
        quitButton.GetComponentInChildren<TMP_Text>().fontSize = buttonTextFontSize;


    }
    #endregion

    #region End Game
    public void EndGame()
    {
        endGame = true;

        endScoreText.text = "Score: " + currentScore;

        //End game screen: Buttons to retry or return to menu
        HUD.SetActive(false);
        EndHUD.SetActive(true);

        //High score is updated if necessary
        if (highScore < currentScore)
        {
            StreamWriter sw = File.CreateText("highscore.txt");

            sw.WriteLine(currentScore.ToString());
            
            sw.Close();
        }
    }

    public void Retry()
    {
        //Reload the current game scene
        SceneManager.LoadScene("Gameplay");
    }

    public void ExitToMenu()
    {
        //Load the main menu
        SceneManager.LoadScene("MainMenu");
    }

    public void QuitApp()
    {
        //Close the application
        Application.Quit();
    }
    #endregion
}
