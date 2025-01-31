// Get references to the slider and input fields
const slider1 = document.getElementById("slider1");
const input1 = document.getElementById("input1");
const slider2 = document.getElementById("slider2");
const input2 = document.getElementById("input2");
const warning1 = document.getElementById("warning1");
const warning2 = document.getElementById("warning2");

// Update input field when the slider value changes
slider1.addEventListener("input", function() {
	input1.value = slider1.value;
	if(input1.value == 0) {
		warning1.style.visibility = "visible";
	}
	else {
		warning1.style.visibility = "hidden";
	}
});

slider2.addEventListener("input", function() {
	input2.value = slider2.value;
	if(input2.value == 0) {
		warning2.style.visibility = "visible";
	}
	else {
		warning2.style.visibility = "hidden";
	}
});

// Update slider when the input field value changes
input1.addEventListener("input", function() {
	const value = Math.min(Math.max(input1.value, 0), 300); // Ensure it's within bounds
	slider1.value = value;
	input1.value = value; // To update the input field if it's manually changed
	if(input1.value == 0) {
		warning1.style.visibility = "visible";
	}
	else {
		warning1.style.visibility = "hidden";
	}
});

input2.addEventListener("input", function() {
	const value = Math.min(Math.max(input2.value, 0), 300); // Ensure it's within bounds
	slider2.value = value;
	input2.value = value; // To update the input field if it's manually changed
	if(input2.value == 0) {
		warning2.style.visibility = "visible";
	}
	else {
		warning2.style.visibility = "hidden";
	}
});

/*

document.getElementById('sendBtn').addEventListener('click', function() {
  // Get the values from the input fields in popup.html
  const input1 = document.getElementById('input1').value;
  const input2 = document.getElementById('input2').value;

	
  // Send the values to the content script via message passing
  chrome.tabs.query({ active: true, currentWindow: true }, function(tabs) {
    // Send a message to the content script of the active tab
    chrome.tabs.sendMessage(tabs[0].id, {
      type: 'SET_TIMERS',
      input1: input1,
      input2: input2
    });
  });
});

document.getElementById('refresh').addEventListener('click', function() {
  chrome.tabs.query({ active: true, currentWindow: true }, function(tabs) {
    // Send a message to the content script of the active tab
    chrome.tabs.sendMessage(tabs[0].id, {
      type: 'REFRESH'
    });
  });
});

*/