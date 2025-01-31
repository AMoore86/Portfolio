chrome.tabs.onUpdated.addListener((tabId, changeInfo, tab) => {
  // Check if the tab's URL has finished loading
  if (changeInfo.status === 'complete') {
    // The string you want to match in the URL
    const targetString = "/shorts/";
    
    // If the current URL contains the target string, redirect
    if (tab.url.includes(targetString)) {
      const newUrl = "https://www.youtube.com"; // The URL you want to redirect to
      chrome.tabs.update(tabId, { url: newUrl });
    }
  }
});
