var heading = document.getElementById("heading");
var textEdit = document.getElementById("textEdit");
var button = document.getElementById("button");
var form = document.getElementById("form");
var form1 = document.getElementById("form1");
var form2 = document.getElementById("form2");
var addHeading = document.getElementById("add");
var addText = document.getElementById("addT");
var addButton = document.getElementById("addB");
var clear = document.getElementById("clearAll");
var createdForm = document.getElementById("example");
var previousTab = heading;
var previousForm = form;

clear.addEventListener("click", function() {
    document.getElementById("createdHead").innerHTML = "";
    if (document.getElementById("created").hasChildNodes()) {
        document.getElementById("created").innerHTML = "";
    }  
}, false);
addHeading.addEventListener("click", function() {
    if (document.getElementById("headingBox") != null) {
        var h = document.getElementById("headingBox").value;
        document.getElementById("createdHead").innerHTML = h;
        if (document.getElementById("italic").checked) {
            document.getElementById("createdHead").style.fontStyle = "italic";
        } else {
            document.getElementById("createdHead").style.fontStyle = "normal";
        }
    }
}, false);
addText.addEventListener("click", function() {
    var textBoxTitle = document.getElementById("inputTitle").value;
    var textBoxInitial = document.getElementById("initialVal").value;
    console.log(textBoxInitial);
    var newTextBox = document.createElement("input");
    var newTitle = document.createElement("label");
    newTitle.innerText = textBoxTitle;
    newTextBox.type = "text";
    newTextBox.value = textBoxInitial;
    document.getElementById("created").appendChild(newTitle);
    document.getElementById("created").appendChild(newTextBox);
    document.getElementById("created").appendChild(document.createElement("BR"));
}, false);
addButton.addEventListener("click", function() {
    var buttonText = document.getElementById("buttonT").value;
    var newButton = document.createElement("button");
    newButton.innerHTML = buttonText;
    document.getElementById("created").appendChild(newButton);
    document.getElementById("created").appendChild(document.createElement("BR"));
}, false);
heading.addEventListener("click", function() {
    previousTab.style.border = "solid white 4px";
    heading.style.border = "solid black 4px";
    previousForm.style.display = "none";
    form.style.display = "block";
    previousTab = heading;
    previousForm = form;
}, false);
textEdit.addEventListener("click", function() {
    previousTab.style.border = "solid white 4px";
    textEdit.style.border = "solid black 4px";
    previousForm.style.display = "none";
    form1.style.display = "block";
    previousTab = textEdit;
    previousForm = form1;
}, false);
button.addEventListener("click", function() {
    previousTab.style.border = "solid white 4px";
    button.style.border = "solid black 4px";
    previousForm.style.display = "none";
    form2.style.display = "block";
    previousTab = button;
    previousForm = form2;
}, false);