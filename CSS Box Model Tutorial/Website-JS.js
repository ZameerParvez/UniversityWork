//ES6
/*
//This method changes the size of the padding of the given element to the given size
const changeSize = (element, sizeInput) => {
  const size = parseInt(sizeInput.value);
  //If the size entered is bigger than the maximum set then set it tpo the maximimum
  if (size>sizeInput.max) {
    element.style.padding = sizeInput.max + 'px';
    sizeInput.value = 60;
    return false;
  }
  element.style.padding = size + 'px';
}

//This method writes to the content box with the input content
const changeText = (toReplace, input) => {
  toReplace.value = input.value;
}

//This function allows the navbar to only display one section of the page at a time
const clearAllBut = element => {
  const content = document.getElementsByClassName('Content');
  for (var i = 0; i < content.length; i++) {
    content[i].style.display = "none";
  }
  element.style.display = "block";
}*/
 //ES5 Is used for better compatability with browsers
 'use strict';
//This method changes the size of the padding of the given element to the given size
function changeSize(element, sizeInput) {
   var size = parseInt(sizeInput.value);
   //If the size entered is bigger than the maximum set then set it tpo the maximimum
   if (size > sizeInput.max) {
     element.style.padding = sizeInput.max + 'px';
     sizeInput.value = 60;
     return false;
   }
   element.style.padding = size + 'px';
 };

//This method writes to the content box with the input content
function changeText(toReplace, input) {
   toReplace.value = input.value;
 };

//This function allows the navbar to only display one section of the page at a time
function clearAllBut(element) {
   var content = document.getElementsByClassName('Content');
   for (var i = 0; i < content.length; i++) {
     content[i].style.display = "none";
   }
   element.style.display = "block";
 };
