using UnityEngine;
using System.Collections;

public class NewBehaviourScript : MonoBehaviour {

	public float speed = 3.5f;
	public float rotatingSpeed = 40f;
	public float jumpingForce = 10f;
	public bool canJump;
	// Use this for initialization
	void Start () {
	
	}
	
	// Update is called once per frame
	void Update () {
		if(Input.GetKey("right")){
			transform.RotateAround(transform.position, Vector3.up,rotatingSpeed * speed * Time.deltaTime); 
		}
		if(Input.GetKey("left")){
			transform.RotateAround(transform.position, Vector3.up,-rotatingSpeed * speed * Time.deltaTime);
		}
		if(Input.GetKeyDown("space")){
			canJump=false;
			GetComponent<Rigidbody> ().AddForce(0,jumpingForce,0);
		}
		if(Input.GetKey("up")){
			transform.position +=transform.forward * speed * Time.deltaTime;
		}
		if(Input.GetKey("down")){
			transform.position -=transform.forward * speed * Time.deltaTime;
		}



	
	}
	//void onCollisionEnter (Collision collision){
		//if(collision.transform.name =="floor"){
			//canJump=True;
	//	}
		

	//}
}
