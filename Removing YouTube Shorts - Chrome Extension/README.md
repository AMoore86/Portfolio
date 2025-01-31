# Removing YouTube Shorts - Chrome Extension
## Goal
The goal of this project is to help with personal time management. I watch quite a lot of YouTube in my free time, and ever since they have added YouTube Shorts to their application, it has negatively affected my time management. There have been many instances where I have found myself scrolling through YouTube Shorts without thinking, only to realize that I have wasted large amounts of time. This Google Chrome extension, intened for personal use, is designed to immediately remove any signs of YouTube Shorts from the html of their webpage when it is loaded. Additionally, if I somehow find myself navigated to a YouTube Short, it is designed to redirect me to the home page of the website.

## The Extension
### Background Application
The extension utilizes javascript that checks the URL for a particular string: '/shorts/'. This string is used because it is used by YouTube to denote the Shorts videos category on their website. This check is done each time the current tab has completed its loading process. If that target string is included in the URL of that tab, the extension will automatically redirect the page to the main home page of the site: www.youtube.com

### Active Deletion
The extension also checks for HTML elements with specific classes, and deletes their (grand)parent nodes which effectively removes any possibility of clicking on a YouTube Short or on the button for the "Shorts" category. These classes were chosen by inspecting the elements on the YouTube home page. This deletion also occurs after the tab has completed its loading process, and takes a few moments.

### Popup Window
Clicking on the extension will open a small window which currently has a couple sliders, input fields, and buttons on it. The intention of these items was to allow the user to configure how long the extension would run in the background after each page loaded, as well as how often the extension would check for addtitional Shorts-related material on the web page. However, this UI is currently inactive, and those settings are hardcoded to work within a few moments each time the tab loads a new page.

### Further Improvements
The following is a list of potential improvements to the Google Chrome extension:
- Complete the connection between the popup window UI and the active deletion
  - Timing configuration
  - Refresh Button
  - Button to trigger active deletion
- Update the popup window CSS file to make the UI more pleasing to look at
- Implement the deletion and/or hiding of other HTML elements to help streamline my YouTube viewing experience
