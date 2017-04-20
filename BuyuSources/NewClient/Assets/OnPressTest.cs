using UnityEngine;
using System.Collections;

public class OnPressTest : MonoBehaviour {

	// Use this for initialization
	void Start () {
        UICamera.onPress = OnPress;
	}
	
	// Update is called once per frame
	void Update () {
	
	}

    void OnPress(GameObject go, bool pressed)
    {
        Debug.Log(go.name+","+pressed);
    }
}
