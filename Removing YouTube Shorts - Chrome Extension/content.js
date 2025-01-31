// Set initial values for your variables
let maxRuntime = 5000;
let timeInterval = 250;

/*
// Listen for messages from popup.js
chrome.runtime.onMessage.addListener(function(message, sender, sendResponse) => {
  if (message.type === 'SET_VARIABLES') {
    // Update the variables with the data received from the popup
    maxRuntime = message.input1 * 1000;
    timeInterval = message.input2 * 1000;
		
		if (timeInterval == 0) {
			timeInterval = maxRuntime - 1;
		}

    // Log the values for testing
    console.log('Max Runtime:', maxRuntime);
    console.log('Time Interval:', timeInterval);
  }
	if (message.type === 'REFRESH') {
		chrome.tabs.reload(sender.tab.id);
	}
});
*/

var interval = setInterval(function() {
	var shorts = document.querySelectorAll('.shortsLockupViewModelHost');
	var shortsIcons = document.querySelectorAll('a[title="Shorts"]');
	
	//Do not run the rest of the script if there are no shorts
	if(shorts.length < 1 && shortsIcons.length < 1) {
		return;
	}
	if(maxRuntime == 0) {
		clearTimeout(timeout);
	}
	
	//Remove any shorts from displaying
	shorts.forEach(function(element) {
		console.log("There are " + shorts.length + " shorts in the document.");
		var modal = element.parentNode.parentNode.parentNode.parentNode.parentNode.parentNode;
		if(modal) {
			modal.remove();
		}
	});
	
	//Remove the Shorts Icon navigation option
	shortsIcons.forEach(function(shortsIcon) {
		console.log("There are " +shortsIcons.length+ " shorts icons.");
		var modal = shortsIcon.parentNode;
		if (modal) {
			modal.remove();
		}
	});
	
	//No longer run this script after specified time (milliseconds)
	let timeout = setTimeout(function() {
    clearInterval(interval);
		console.log("Auto Shorts Deletion is no longer working");
  }, maxRuntime);
	//Run the script every specified x milliseconds
}, timeInterval);
