using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Drop : MonoBehaviour
{
    [HideInInspector]
    public bool hitSomething;
    private bool spawned;

    private Gameplay gp;
    private int currentLevel, pointIncrement;

    [SerializeField]
    private GameObject starPrefab;

    // Start is called before the first frame update
    void Start()
    {
        gp = Camera.main.gameObject.GetComponent<Gameplay>();

        char[] separators = {' ', '(', ')'};
        currentLevel = System.Int32.Parse(gameObject.name.Split(separators)[1]);
    }

    private void OnCollisionEnter2D(Collision2D collision)
    {
        hitSomething = true;

        //End game if ball hits the floor
        if (collision.collider.gameObject.layer == 8 && !gp.endGame)
            gp.EndGame();

        //Return if Saturn (using Trigger for Saturn's upgrade)
        if (currentLevel == 9 || (currentLevel == 10 && !gp.easyMode))
            return;

        //Return if one of the objects do not have a rigidbody
        if (gameObject.GetComponent<Rigidbody2D>() == null || collision.collider.gameObject.GetComponent<Rigidbody2D>() == null)
            return;

        //Bugfix for objects colliding and both having zero velocity
        if(gameObject.GetComponent<Rigidbody2D>().velocity.magnitude 
            == collision.collider.gameObject.GetComponent<Rigidbody2D>().velocity.magnitude)
        {
            gameObject.GetComponent<Rigidbody2D>().velocity += new Vector2(Random.Range(0.001f, 0.001f), Random.Range(0.001f, 0.001f));
        }

        //Handle collisions with same-type objects
        //Making sure only one of the colliding objects runs this collision code
        if(!spawned && collision.collider.name == gameObject.name 
            && gameObject.GetComponent<Rigidbody2D>().velocity.magnitude 
            > collision.collider.gameObject.GetComponent<Rigidbody2D>().velocity.magnitude)
        {
            spawned = true;
            
            //Destroy colliding objects
            Destroy(collision.collider.gameObject);

            //Spawn the next-level object at the point of contact
            if (currentLevel < 10)
            {
                GameObject newProduct = Instantiate(gp.drops[currentLevel], collision.contacts[0].point, Quaternion.identity, gp.dropsSpawned);
                newProduct.layer = 7;
            }
            else
                Instantiate(starPrefab, collision.contacts[0].point, Quaternion.identity, gp.background);

            //Increment score according to next-level object
            if(!gp.endGame)
                gp.currentScore += (currentLevel + 1);

            Destroy(gameObject);
        }

    }

    private void OnTriggerEnter2D(Collider2D collision)
    {
        //Return if one of the objects do not have a rigidbody
        if (gameObject.GetComponent<Rigidbody2D>() == null || collision.gameObject.GetComponent<Rigidbody2D>() == null)
            return;

        //Bugfix for objects colliding and both having zero velocity
        if (gameObject.GetComponent<Rigidbody2D>().velocity.magnitude
            == collision.gameObject.GetComponent<Rigidbody2D>().velocity.magnitude)
        {
            gameObject.GetComponent<Rigidbody2D>().velocity += new Vector2(Random.Range(0.001f, 0.001f), Random.Range(0.001f, 0.001f));
        }

        //Handle collisions with same-type objects
        //Making sure only one of the colliding objects runs this collision code
        if (!spawned && collision.name == gameObject.name
            && gameObject.GetComponent<Rigidbody2D>().velocity.magnitude
            > collision.gameObject.GetComponent<Rigidbody2D>().velocity.magnitude)
        {
            spawned = true;
            
            //Destroy colliding objects
            Destroy(collision.gameObject);

            //Spawn the next-level object at the point of contact
            Vector3 newProductLocation = collision.ClosestPoint(transform.position);
            GameObject newProduct = Instantiate(gp.drops[currentLevel], newProductLocation, Quaternion.identity, gp.dropsSpawned);
            newProduct.layer = 7;

            //Increment score according to next-level object
            if (!gp.endGame)
                gp.currentScore += (int)Mathf.Pow(2f, currentLevel);

            Destroy(gameObject);
        }
    }
}
