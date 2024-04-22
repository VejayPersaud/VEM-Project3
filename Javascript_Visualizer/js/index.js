//Listens for the Document Object Model to be fully loaded before executing the functions inside
document.addEventListener('DOMContentLoaded', function () {
    generateBars(); //Generates the initial set of bars when the page loads

    //Event listener for the "Generate New Array" button to be clicked
    document.getElementById('generateArray').addEventListener('click', generate);

    //Event listener for selection sort button
    document.getElementById('selectionSort').addEventListener('click', function () {
        disableButtons(); //Buttons turned off during the sort
       selectionSort(); //Starts the sorting algorithm
    });


    //Event listener for merge sort button
    document.getElementById('mergeSort').addEventListener('click', async function () {
        disableButtons(); //Buttons off
        await mergeSort(); //waits for sort to finish
        enableButtons(); //Buttons turned back on
    });

    //Event listener for heap sort button
    document.getElementById('heapSort').addEventListener('click', async function () {
        disableButtons(); //Buttons off
        await heapSort(); //wait for sort to finish
        enableButtons(); //buttons back on
    });

    //Event listener for quick sort button
    document.getElementById('quickSort').addEventListener('click', async function () {
        disableButtons(); //Buttons off
        let bars = document.querySelectorAll(".bar"); //select elements of the bar class
        let l = 0; //left index is 0
        let r = parseInt(bars.length) - 1; //right index is last index of the bars array
        await quickSort(bars, l, r); //starts quick sort and waits for completion
        enableButtons(); //Buttons back on
    });


    //Sources
    //Title: Selection Sort Visualizer in Javascript
    //CSS,HTML, and Selection sort used as base to stylize and build project
    //Reference Pages: https://www.geeksforgeeks.org/selection-sort-visualizer-in-javascript/
});

function generateBars(num = 20) {
    const container = document.querySelector(".data-container");
    container.innerHTML = ''; //Clear the container
    const uniqueHeights = new Set(); //Hold the unique heights

    //Maximum bar height
    const maxHeight = 100;

    //Generate unique bars until we have num unique heights
    while (uniqueHeights.size < num) {
        const value = Math.floor(Math.random() * maxHeight) + 1;
        if (!uniqueHeights.has(value)) { //Checks the set to ensure unique height
            uniqueHeights.add(value); //Add height to the set 
            const bar = createBar(value, uniqueHeights.size - 1); //Calls create bar to make new bar element
            container.appendChild(bar); //Add the bar to the container 
        }
    }

    //Sources
    //Title: Selection Sort Visualizer in Javascript
    //CSS,HTML, and Selection sort used as base to stylize and build project
    //Reference Pages: https://www.geeksforgeeks.org/selection-sort-visualizer-in-javascript/
}

//Creates a new bar with an height and index
function createBar(value, index) {
    const bar = document.createElement("div"); //Make new div element
    bar.classList.add("bar"); //Add class "bar" to the div
    bar.style.height = `${value * 3}px`; //Set the height of bar based on value
    bar.style.transform = `translateX(${index * 30}px)`; //set horizontal position of bar
    const barLabel = document.createElement("label"); //Make a label element for the bar value
    barLabel.classList.add("bar_id"); //Add class "bar_id" to the label
    barLabel.innerHTML = value; //Set the text of label to bar value
    bar.appendChild(barLabel); //Add the label to the bar
    return bar;

    //Sources
    //Title: Selection Sort Visualizer in Javascript
    //CSS,HTML, and Selection sort used as base to stylize and build project
    //Reference Pages: https://www.geeksforgeeks.org/selection-sort-visualizer-in-javascript/
}

//Quicksort on bars container
async function quickSort(bars, low, high) {
    if (low < high) {
        //Perform the partition on the array and get the index of pivot
        let pivot = await partition(bars, low, high);

        //the pivot is in the correct position
        bars[pivot].style.backgroundColor = 'green';

        //Recursively apply the same logic to the left and right subarrays.
        await quickSort(bars, low, pivot - 1);
        await quickSort(bars, pivot + 1, high);
    } else {
        //When low is equal to high, it means we're dealing with a single element, color it green
        if (low >= 0 && high < bars.length && low === high) {
            bars[low].style.backgroundColor = 'green';
        }
    }

    //Sources
    //Title: Quick sort function, 4.) Program Details, (translate the page), and p5.js Quick sort
    //Cross referenced approaches to implement quick sort
    //Reference Pages: https://qiita.com/shinonome8090/items/029b2c4cebb550f40853
    //https://www.geeksforgeeks.org/p5-js-quick-sort/?ref=lbp
}

//Partition the array for quick sort
async function partition(bars, low, high) {
    let pivot = bars[high].style.height; //Set the pivot as last element
    let pivotValue = bars[high].childNodes[0].innerText; //Store the pivot value to update later
    let i = low - 1;//Sets the index of the smaller element

    for (let j = low; j < high; j++) {
        // Color current element being compared
        bars[j].style.backgroundColor = 'yellow';

        await new Promise(resolve => setTimeout(resolve, 100)); //Pause 100ms


        //Check if current is smaller than pivot
        if (parseInt(bars[j].style.height) < parseInt(pivot)) {
            i++;

            // Swap heights of bars
            let tempHeight = bars[i].style.height;
            bars[i].style.height = bars[j].style.height;
            bars[j].style.height = tempHeight;

            // Swap values of bars
            let tempValue = bars[i].childNodes[0].innerText;
            bars[i].childNodes[0].innerText = bars[j].childNodes[0].innerText;
            bars[j].childNodes[0].innerText = tempValue;

            // Color the bars that have been swapped
            bars[i].style.backgroundColor = 'orange';
            if (i !== j) bars[j].style.backgroundColor = 'pink'; //Reset the color of bar j
        } else {
            bars[j].style.backgroundColor = 'pink'; //Reset the color if no swap was needed
        }
    }

    //Put the pivot in the correct position by swapping with bar at i + 1
    bars[high].style.height = bars[i + 1].style.height;
    bars[i + 1].style.height = pivot;
    bars[high].childNodes[0].innerText = bars[i + 1].childNodes[0].innerText;
    bars[i + 1].childNodes[0].innerText = pivotValue;

    //The pivot bar is now at index i+1, color it as part of the sorted partition
    bars[i + 1].style.backgroundColor = 'green';

    await new Promise(resolve => setTimeout(resolve, 100));//Pause 100ms again, this helps the animation feel more fluid
    return i + 1; //Return the partition index

    //Sources
    //Title: Quick sort function, 4.) Program Details, (translate the page), and p5.js Quick sort
    //Cross referenced approaches to implement quick sort 
    //Reference Pages: https://qiita.com/shinonome8090/items/029b2c4cebb550f40853
    //https://www.geeksforgeeks.org/p5-js-quick-sort/?ref=lbp

}



//Heap Sort on bars container
async function heapSort() {
    let bars = document.querySelectorAll(".bar");
    let n = bars.length; 

    //Build heap
    for (let i = Math.floor(n / 2) - 1; i >= 0; i--) {
        await heapify(bars, n, i);
    }

    //One by one extracts an element from heap
    for (let i = n - 1; i > 0; i--) {
        
        //Temp variables to prepare for moving
        let tempHeight = bars[0].style.height;
        let tempValue = bars[0].childNodes[0].innerText;

        //Move current to end
        bars[0].style.height = bars[i].style.height;
        bars[0].childNodes[0].innerText = bars[i].childNodes[0].innerText;

        //Restores the displaced value
        bars[i].style.height = tempHeight;
        bars[i].childNodes[0].innerText = tempValue;

        bars[i].style.backgroundColor = 'pink'; //Color to indicate final position
        await new Promise(resolve => setTimeout(resolve, 100)); //Pause 100ms
        //Call max heapify on the reduced heap
        await heapify(bars, i, 0);
    }
    // Ensure the root element gets its final color
    bars[0].style.backgroundColor = 'pink';

    //Sources
    //Title: Heap Sort Visualization in Javascript, Heap sort function, 4.) Program Details, (translate the page)
    //Heap Sort and Heapify from the js file used to help build final product
    //Reference Pages: https://www.geeksforgeeks.org/heap-sort-visualization-using-javascript/?ref=header_search
    //https://qiita.com/shinonome8090/items/029b2c4cebb550f40853
}

async function heapify(bars, n, i) {
    let largest = i; // Initialize largest as root
    let l = 2 * i + 1; // left = 2*i + 1
    let r = 2 * i + 2; // right = 2*i + 2

    //If left child is larger than root
    if (l < n && parseInt(bars[l].style.height) > parseInt(bars[largest].style.height)) {
        largest = l;
    }

    // If right child is larger than largest so far
    if (r < n && parseInt(bars[r].style.height) > parseInt(bars[largest].style.height)) {
        largest = r;
    }

    //If largest is not root
    if (largest !== i) {
        //Swap height and values of the bar
        let tempHeight = bars[i].style.height;
        let tempValue = bars[i].childNodes[0].innerText;
        bars[i].style.height = bars[largest].style.height;
        bars[i].childNodes[0].innerText = bars[largest].childNodes[0].innerText;
        bars[largest].style.height = tempHeight;
        bars[largest].childNodes[0].innerText = tempValue;

        bars[i].style.backgroundColor = 'red'; //Color current root
        bars[largest].style.backgroundColor = 'blue'; //Color largest

        //Recursively heapify the affected sub tree
        await new Promise(resolve => setTimeout(resolve, 100)); //Pause
        await heapify(bars, n, largest);
    }

    //Sources
    //Title: Heap Sort Visualization in Javascript, Heap sort function, 4.) Program Details, (translate the page)
    //Heap Sort and Heapify from the js file used to help build final product
    //Reference Pages: https://www.geeksforgeeks.org/heap-sort-visualization-using-javascript/?ref=header_search
    //https://qiita.com/shinonome8090/items/029b2c4cebb550f40853
}

//Tweaked Selection Sort from Geeks for Geeks, used to model JS workflow for the other sorts
async function selectionSort() {

    //Get all the bar elements
    let bars = document.querySelectorAll(".bar");
    
    for (let i = 0; i < bars.length - 1; i++) {//Go through all except last bar
        let minIndex = i; //Current as mininum index
        bars[i].style.backgroundColor = "darkblue";//Dark blue for currently being processed

        for (let j = i + 1; j < bars.length; j++) {//Iterate through remaining unsorted bars
            bars[j].style.backgroundColor = "red"; //Red for currently being compared
            await new Promise(resolve => setTimeout(resolve, 100)); //Pause

            //Get the values of current and minimum bars
            let currentVal = parseInt(bars[j].childNodes[0].innerHTML);
            let minVal = parseInt(bars[minIndex].childNodes[0].innerHTML);

            if (currentVal < minVal) {//If smaller value found, update the minimum
                if (minIndex !== i) {
                    bars[minIndex].style.backgroundColor = "rgb(0, 183, 255)"; //Reset previous min bar color
                }
                minIndex = j;
            } else {
                bars[j].style.backgroundColor = "rgb(0, 183, 255)"; //If current bar is not new min, reset the color 
            }
        }

        //Swap height and labels of current and minimum bar
        let tempHeight = bars[minIndex].style.height;
        let tempLabel = bars[minIndex].childNodes[0].innerHTML;
        bars[minIndex].style.height = bars[i].style.height;
        bars[i].style.height = tempHeight;
        bars[minIndex].childNodes[0].innerHTML = bars[i].childNodes[0].innerHTML;
        bars[i].childNodes[0].innerHTML = tempLabel;


        //Change color of min and current bar to show the swap
        bars[minIndex].style.backgroundColor = "rgb(0, 183, 255)";
        bars[i].style.backgroundColor = "rgb(49, 226, 13)";

        await new Promise(resolve => setTimeout(resolve, 100)); //Pause for animation
    }
    enableButtons();

    //Sources
    //Title: Selection Sort Visualizer in Javascript
    //CSS,HTML, and Selection sort used as base to stylize and build project
    //Reference Pages: https://www.geeksforgeeks.org/selection-sort-visualizer-in-javascript/
    


}


async function mergeSort(bars = document.querySelectorAll(".bar"), l = 0, r = document.querySelectorAll(".bar").length - 1) {
    if (l >= r) {
        return; //Returns recursively only if left greater or equal to right
    }
    const m = l + parseInt((r - l) / 2); //Store middle index
    await mergeSort(bars, l, m); //Sort left
    await mergeSort(bars, m + 1, r); //Sort right
    await merge(bars, l, m, r); //Merge the halves

    //Source
    //Title: Sort Visualizer and GFG Merge Sort visualization
    //Java Implementation
    //Reference Pages: https://www.sortvisualizer.com/mergesort/
    //https://www.geeksforgeeks.org/merge-sort-visualization-in-javascript/?ref=header_search
}

async function merge(bars, l, m, r) {
    const n1 = m - l + 1; //Store size of left subarray
    const n2 = r - m; //Store size of right subarray

    //Creates temporary subarrays for the left and right
    let left = new Array(n1);
    let right = new Array(n2);

    //Copy to left subarray
    for (let i = 0; i < n1; i++) {
        left[i] = bars[l + i].style.height;
        bars[l + i].style.backgroundColor = 'yellow'; // Color left partition
    }

    //Copy to right subarray
    for (let j = 0; j < n2; j++) {
        right[j] = bars[m + 1 + j].style.height;
        bars[m + 1 + j].style.backgroundColor = 'orange'; // Color right partition
    }

    await new Promise(resolve => setTimeout(resolve, 100));//Anim. Pause


    //Marge the left and right subarrays
    let i = 0, j = 0, k = l;
    while (i < n1 && j < n2) {
        if (parseInt(left[i]) <= parseInt(right[j])) {
            bars[k].style.height = left[i];
            i++;
        } else {
            bars[k].style.height = right[j];
            j++;
        }
        bars[k].childNodes[0].innerHTML = bars[k].style.height.replace('px', '') / 3;
        bars[k].style.backgroundColor = 'green'; //Color merged elements green
        k++;

        await new Promise(resolve => setTimeout(resolve, 100)); //Anim. Pause
    }

    //Copy remaining left subarray elements if they exist
    while (i < n1) {
        bars[k].style.height = left[i];
        bars[k].childNodes[0].innerHTML = bars[k].style.height.replace('px', '') / 3;
        bars[k].style.backgroundColor = 'green';
        i++;
        k++;

        await new Promise(resolve => setTimeout(resolve, 100)); //Pause
    }


    //Copy remaining right subarray elements if they exist
    while (j < n2) {
        bars[k].style.height = right[j];
        bars[k].childNodes[0].innerHTML = bars[k].style.height.replace('px', '') / 3;
        bars[k].style.backgroundColor = 'green';
        j++;
        k++;

        await new Promise(resolve => setTimeout(resolve, 100));//Pause
    }

    //Source
    //Title: Sort Visualizer and GFG Merge Sort visualization
    //Java Implementation
    //Reference Pages: https://www.sortvisualizer.com/mergesort/
    //https://www.geeksforgeeks.org/merge-sort-visualization-in-javascript/?ref=header_search




}


function disableButtons() {
    document.querySelectorAll('.btn').forEach(btn => {
        btn.disabled = true;
        btn.style.backgroundColor = "#d8b6ff";
    });
}

function enableButtons() {
    document.querySelectorAll('.btn').forEach(btn => {
        btn.disabled = false;
        btn.style.backgroundColor = "#6f459e";
    });
}

function generate() {
    window.location.reload();
}



