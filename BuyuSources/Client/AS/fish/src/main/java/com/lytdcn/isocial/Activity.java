package com.lytdcn.isocial;

import com.unity3d.player.*;
import com.util.IabHelper;
import com.util.IabResult;
import com.util.Inventory;
import com.util.Purchase;

import android.app.AlertDialog;
import android.os.Bundle;
import android.util.Log;

public class Activity extends UnityPlayerActivity {

	private String mCallbackObj = "SDKCallbackObj";
	private String mCallbackMethod = "GPCallback";

	// ___________________________________________________________
	// The helper object
	IabHelper mHelper;

	// Debug tag, for logging
	static final String TAG = "fish";

	// Does the user have the premium upgrade?
	boolean mIsPremium = false;

	// Does the user have an activesubscription to the infinite gas plan?
	boolean mSubscribedToInfiniteGas = false;

	// SKUs for our products: the premiumupgrade (non-consumable) and gas
	// (consumable)
	static String SKU_consume = "";
	static String SKU_noconsume = "";

	// static final String SKU_GAS="";
	// SKU for our subscription (infinite gas)

	// static final String SKU_INFINITE_GAS ="infinite_gas";
	// (arbitrary) request code for the purchaseflow

	static final int RC_REQUEST = 10001;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);

		String base64EncodedPublicKey = "MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAj1OYX8daVsCD2Z9GrNHc4ZCZsAceOK1w21ZkWZaBqp02yBkvAUHWd5AUuooMF5ay1cfW93J7dnuXIvWqOFKlOjV+ZGAb7NwT62MM7v0qGC799vo6/+sRYY033Qyfor6WOsjlnJORUPVUzUX449IJ57y4rRv6k0JeDvQoWBKfZUATtQJOqQod1q9lSHaYqZsFfFxhT++WVa87PG5pBZxr6fnRCeVvo6qZHKyEAj8LgMkhGLeLEOqU/3ebfPLi3CVo7e2MoPXpcyGx7YREo28klOzkeSv/JFmeABat1YBu15e3B7aNMFWZjSExyXwfNaCV1gF7yE4R3vvmQ7KS1ePMvQIDAQAB";

		// Create the helper, passing it our contextand the public key to verify
		// signatures with
		Log.d(TAG, "Creating IABhelper.");
		mHelper = new IabHelper(this, base64EncodedPublicKey);

		// enable debug logging (for aproduction application, you should set
		// this to false).
		mHelper.enableDebugLogging(true);

		// Start setup. This is asynchronousand the specified listener
		// will be called once setup completes.
		Log.d(TAG, "Startingsetup.");
		mHelper.startSetup(new IabHelper.OnIabSetupFinishedListener() {
			public void onIabSetupFinished(IabResult result) {
				Log.d(TAG, "Setupfinished.");

				if (!result.isSuccess()) {
					// Oh noes, there was a problem.
					complain("Problemsetting up in-app billing: " + result);
					return;
				}

				// Have we been disposed of inthe meantime? If so, quit.
				if (mHelper == null)
					return;

				// IAB is fully set up. Now, let'sget an inventory of stuff we
				// own.
				Log.d(TAG, "Setupsuccessful. Querying inventory.");
				mHelper.queryInventoryAsync(mGotInventoryListener);

			}
		});
	}

	@Override
	public void onDestroy() {
		super.onDestroy();
		if (mHelper != null) mHelper.dispose();
		mHelper = null;
	}

	public void Pay(final String buykey, final String callbackObj,
					final String callbackMothod) {

		mCallbackObj = callbackObj;
		mCallbackMethod = callbackMothod;

		SKU_consume = "com.lytdcn.isocial.g_001";

		mHelper.launchPurchaseFlow(this, "com.lytdcn.isocial.g_001", RC_REQUEST,
				mPurchaseFinishedListener);
	}

	// Listener that's called when we finish querying the items and
	// subscriptions weown 购买侦听器完成
	IabHelper.OnIabPurchaseFinishedListener mPurchaseFinishedListener = new IabHelper.OnIabPurchaseFinishedListener() {
		public void onIabPurchaseFinished(IabResult result, Purchase purchase) {
			Log.d(TAG, "Purchase finished: " + result + ", purchase:"
					+ purchase);
			if (result.isFailure()) {

				// complain("Error purchasing: " +result);
				// setWaitScreen(false);
				return;
			}
			if (!verifyDeveloperPayload(purchase)) {
				complain("Error purchasing.Authenticity verification failed.");
				// setWaitScreen(false);
				return;
			}

			Log.d(TAG, "Purchase successful.");

			if (purchase.getSku().equals(SKU_consume)) {
				Log.d(TAG, "Purchase is gas.Starting gas consumption.");
				mHelper.consumeAsync(purchase, mConsumeFinishedListener);
			} else if (purchase.getSku().equals(SKU_noconsume)) {
				Log.d(TAG, "Purchase ispremium upgrade. Congratulating user.");
				alert("Thank you forupgrading to premium!");
				mIsPremium = true;
			}
		}
	};

	// Listener that's called when we finishquerying the items and subscriptions
	// we own
	IabHelper.QueryInventoryFinishedListener mGotInventoryListener = new IabHelper.QueryInventoryFinishedListener() {
		public void onQueryInventoryFinished(IabResult result,
											 Inventory inventory) {
			Log.d(TAG, "Query inventoryfinished.");
			// Have we been disposed of in themeantime? If so, quit.
			if (mHelper == null)
				return;
			// Is it a failure?
			if (result.isFailure()) {
				complain("Failed to queryinventory: " + result);
				return;
			}

			Log.d(TAG, "Query inventorywas successful.");

			/*
			 * Check for items we own. Noticethat for each purchase, we check
			 * the developer payload to see ifit's correct! See
			 * verifyDeveloperPayload().
			 */

			// Do we have the premium upgrade?
			Purchase premiumPurchase = inventory.getPurchase(SKU_noconsume);
			mIsPremium = (premiumPurchase != null && verifyDeveloperPayload(premiumPurchase));
			Log.d(TAG, "User is " + (mIsPremium ? "PREMIUM" : "NOT PREMIUM"));

			// Check for gas delivery -- if weown gas, we should fill up the
			// tank immediately
			Purchase gasPurchase = inventory.getPurchase(SKU_consume);
			if (gasPurchase != null && verifyDeveloperPayload(gasPurchase)) {
				Log.d(TAG, "We have gas.Consuming it.");
				mHelper.consumeAsync(inventory.getPurchase(SKU_consume),
						mConsumeFinishedListener);
				return;
			}

			Log.d(TAG, "Initial inventoryquery finished; enabling main UI.");
		}
	};
	// Called when consumption is complete
	IabHelper.OnConsumeFinishedListener mConsumeFinishedListener = new IabHelper.OnConsumeFinishedListener() {
		public void onConsumeFinished(Purchase purchase, IabResult result) {
			Log.d(TAG, "Consumptionfinished. Purchase: " + purchase
					+ ", result: " + result);

			// if we were disposed of in themeantime, quit.
			if (mHelper == null)
				return;

			// We know this is the"gas" sku because it's the only one we
			// consume,
			// so we don't check which sku wasconsumed. If you have more than
			// one
			// sku, you probably shouldcheck...
			if (result.isSuccess()) {
				// successfully consumed, so weapply the effects of the item in
				// our
				// game world's logic, which inour case means filling the gas
				// tank a bit
				Log.d(TAG, "Consumptionsuccessful. Provisioning.");
			} else {
				complain("Error whileconsuming: " + result);
			}

			Log.d(TAG, "End consumptionflow.");
		}
	};

	/** Verifies the developer payload of apurchase. */
	boolean verifyDeveloperPayload(Purchase p) {
		String payload = p.getDeveloperPayload();

		/*
		 * TODO: verify that the developerpayload of the purchase is correct. It
		 * will be the same one that you sent wheninitiating the purchase.
		 *
		 * WARNING: Locally generating a randomstring when starting a purchase
		 * and verifying it here might seem like agood approach, but this will
		 * fail in the case where the user purchases anitem on one device and
		 * then uses your app on a different device, because on theother device
		 * you will not have access to the random string you
		 * originallygenerated.
		 *
		 * So a good developer payload hasthese characteristics:
		 *
		 * 1. If two different users purchasean item, the payload is different
		 * between them, so that one user's purchase can't be replayed to
		 * another user.
		 *
		 * 2. The payload must be such that youcan verify it even when the app
		 * wasn't the one who initiated the purchase flow (so that items
		 * purchased by the useron one device work on other devices owned by the
		 * user).
		 *
		 * Using your own server to store andverify developer payloads across
		 * app installations is recommended.
		 */

		return true;
	}

	void complain(String message) {
		Log.e(TAG, "**** TrivialDrive Error: " + message);
		alert("Error: " + message);
	}

	void alert(String message) {
		AlertDialog.Builder bld = new AlertDialog.Builder(this);
		bld.setMessage(message);
		bld.setNeutralButton("OK", null);
		Log.d(TAG, "Showing alert dialog: " + message);
		bld.create().show();
	}

	// ___________________________________________________________

	// 向unity发送消息
	void SendToUnityMessage(String Sendmessage) {
		UnityPlayer.UnitySendMessage(mCallbackObj, mCallbackMethod,
				Sendmessage);
	}
}
