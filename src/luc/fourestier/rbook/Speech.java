package luc.fourestier.rbook;

import java.util.Locale;

import android.content.Context;
import android.speech.tts.TextToSpeech;
import android.util.Log;

public class Speech {
	private TextToSpeech textToSpeechEngine = null;
	private boolean textToSpeechInitialized = false;
    private static final String TAG = Speech.class.getSimpleName();
    private Context context;
	
	/**
	 * Speech is on!
	 */
	public boolean speechOn = false;
	
	/**
	 * Constructor
	 * @param ctx: App context.
	 */
	public Speech(Context ctx) {
		if (ctx == null) {
			throw new IllegalArgumentException();
		}
		context = ctx;
	}

	/**
	 * Private default constructor
	 */
	private Speech() {
	}

	/**
	 * Start the TTS engine.
	 * @param context: Application context.
	 */
	public void initialize() {
		if ((!textToSpeechInitialized) || (textToSpeechEngine == null)) {
			if (textToSpeechEngine != null) {
				textToSpeechEngine.stop();
				textToSpeechEngine.shutdown();
				textToSpeechEngine = null;
			}
			textToSpeechInitialized = false;
			textToSpeechEngine = new TextToSpeech(context, mTTSInitListener);
		}
	}
	
	/**
	 * Mute the speech.
	 */
	public void mute() {
		if (textToSpeechEngine != null) {
			textToSpeechEngine.stop();
		}
		speechOn = false;
	}
	
	/**
	 * Unmute the speech.
	 */
	public void unMute() {
		initialize();
		speechOn = true;
	}
	
	/**
	 * Speak immediately the text specified.
	 * All other ongoing or queued speechs are stopped/discarded.
	 * @param speech: The speech text.
	 */
	public void speak(String speech) {
		if (textToSpeechInitialized && speechOn) {
			Log.v(TAG, "Speak: " + speech);
			textToSpeechEngine.speak(speech, TextToSpeech.QUEUE_FLUSH, null);
		}
	}
	
	/**
	 * Add a text to speak into the queue. 
	 * It will be processed whenever the other ongoing/queued speechs will be done.
	 * @param speech: The speech text.
	 */
	public void addSpeak(String speech) {
		if (textToSpeechInitialized && speechOn) {
			Log.v(TAG, "addSpeak: " + speech);
			textToSpeechEngine.speak(speech, TextToSpeech.QUEUE_ADD, null);
		}
	}
	
	/**
	 * Destroy the TTS.
	 * initialize() call is then needed again.
	 */
	public void destroy() {
		if (textToSpeechEngine != null) {
			textToSpeechEngine.stop();
			textToSpeechEngine.shutdown();
			textToSpeechEngine = null;
			textToSpeechInitialized = false;
		}
		speechOn = false;
	}
	
	private TextToSpeech.OnInitListener mTTSInitListener = new TextToSpeech.OnInitListener() {
		@Override
		public void onInit(int status) {
	         if(status != TextToSpeech.ERROR){
	        	 textToSpeechEngine.setLanguage(Locale.UK);
	             textToSpeechInitialized = true;
	            }				
		}
	};
}
