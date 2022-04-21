Module['onRuntimeInitialized'] = () => {

    var x, i, j, l, ll, selElmnt, a, b, c;
    /* Look for any elements with the class "custom-select": */
    x = document.getElementsByClassName("custom-select");
    l = x.length;
    for (i = 0; i < l; i++) {
        selElmnt = x[i].getElementsByTagName("select")[0];
        ll = selElmnt.length;
        /* For each element, create a new DIV that will act as the selected item: */
        a = document.createElement("DIV");
        a.setAttribute("class", "select-selected");
        a.innerHTML = selElmnt.options[selElmnt.selectedIndex].innerHTML;
        x[i].appendChild(a);
        /* For each element, create a new DIV that will contain the option list: */
        b = document.createElement("DIV");
        b.setAttribute("class", "select-items select-hide");
        for (j = 1; j < ll; j++) {
            /* For each option in the original select element,
            create a new DIV that will act as an option item: */
            c = document.createElement("DIV");
            c.innerHTML = selElmnt.options[j].innerHTML;
            c.addEventListener("click", function (e) {
                /* When an item is clicked, update the original select box,
                and the selected item: */
                var y, i, k, s, h, sl, yl;
                s = this.parentNode.parentNode.getElementsByTagName("select")[0];
                sl = s.length;
                h = this.parentNode.previousSibling;
                for (i = 0; i < sl; i++) {
                    if (s.options[i].innerHTML === this.innerHTML) {
                        s.selectedIndex = i;
                        h.innerHTML = this.innerHTML;
                        y = this.parentNode.getElementsByClassName("same-as-selected");
                        yl = y.length;
                        for (k = 0; k < yl; k++) {
                            y[k].removeAttribute("class");
                        }
                        this.setAttribute("class", "same-as-selected");
                        break;
                    }
                }
                h.click();
            });
            b.appendChild(c);
        }
        x[i].appendChild(b);
        a.addEventListener("click", function (e) {
            /* When the select box is clicked, close any other select boxes,
            and open/close the current select box: */
            e.stopPropagation();
            closeAllSelect(this);
            this.nextSibling.classList.toggle("select-hide");
            this.classList.toggle("select-arrow-active");
        });
    }

    function closeAllSelect(elmnt) {
        /* A function that will close all select boxes in the document,
        except the current select box: */
        var x, y, i, xl, yl, arrNo = [];
        x = document.getElementsByClassName("select-items");
        y = document.getElementsByClassName("select-selected");
        xl = x.length;
        yl = y.length;
        for (i = 0; i < yl; i++) {
            if (elmnt === y[i]) {
                arrNo.push(i)
            } else {
                y[i].classList.remove("select-arrow-active");
            }
        }
        for (i = 0; i < xl; i++) {
            if (arrNo.indexOf(i)) {
                x[i].classList.add("select-hide");
            }
        }
    }

    /* If the user clicks anywhere outside the select box,
    then close all select boxes: */
    document.addEventListener("click", closeAllSelect);

    /* Generate random data set */
    var restaurants = Module.makeData(1000);

    /* Functionality for apply button*/

    let apply = document.querySelector('.apply');
    const sort = document.querySelector('#sorting');
    const rating = document.querySelector('#rating');
    const distance = document.querySelector('#distance');
    const cuisine = document.querySelector('#cuisine');

    const cuisines = ['Any', 'French', 'American', 'Mexican', 'Italian', 'Japanese', 'Indian',
        'Chinese', 'Ethiopian', 'Lebanese', 'Kosher', 'Thai', 'Spanish', 'Cuban'];

    const sorts = [true, false];

    const ratings = [0, 1, 2, 3, 4];

    const distances = [10000, 1, 2, 5, 10, 20];

    apply.addEventListener("click", getAllItems);

    var sortIndex, ratingIndex, disIndex, cuisineIndex;
    var initialized = false;

    function getAllItems(elmnt) {
        sortIndex = sort.selectedIndex;
        if (sortIndex === 0) {
            alert('please select a sorting preference');
            return;
        } else {
            var isSort = sorts[sortIndex - 1];
        }
        ratingIndex = rating.selectedIndex;
        if (ratingIndex === 0) {
            alert('please select a rating preference');
            return;
        } else {
            var ratingPreference = ratings[ratingIndex - 1];
        }

        disIndex = distance.selectedIndex;
        if (disIndex === 0) {
            alert('please select a rating preference');
            return;
        } else {
            var disPreference = distances[disIndex - 1];
        }

        cuisineIndex = cuisine.selectedIndex;
        if (cuisineIndex === 0) {
            alert('please select a cuisine preference');
            return;
        } else {
            var cuisPreference = cuisines[cuisineIndex - 1];
        }

        /* Call function to assign similarity scores */

        Module.setSimScores(restaurants, disPreference, ratingPreference, cuisPreference, isSort);

        /* Call respective sorting algorithm and return performance*/
        var sortType = document.getElementById('sort-type');
        if (sortType.checked) {
            sortType = 'Merge Sort';
        }
        else {
            sortType = 'Quick Sort';
        }

        var sortName = document.getElementById('sort-name');
        var time = document.getElementById('time');
        sortName.innerHTML = sortType;

        var startTime = performance.now();

        if (sortType === 'Merge Sort') {
            Module.MergeSort(restaurants, 0, 1000-1);
        }
        else {
            Module.QuickSort(restaurants, 0, 1000-1);
        }

        var endTime = performance.now();
        time.innerHTML = (endTime - startTime).toFixed(2) + ' ms';


        /* For first 100 items of array, display a div containing restaurant data */
        var resultsForm = document.getElementById("results");
        if (!initialized) {
            for (var i = 0; i < 100; i++) {
                var div = document.createElement('div');
                var nameText = document.createElement('p');
                nameText.innerHTML = 'Name: ' + restaurants.get(i).name;
                nameText.setAttribute('id', 'restaurant-name');
                var ratText = document.createElement('p');
                ratText.innerHTML = 'Rating: ' + restaurants.get(i).rating;
                ratText.setAttribute('id', 'rating-text');
                var disText = document.createElement('p');
                disText.innerHTML = 'Distance: ' + restaurants.get(i).distance.toFixed(2);
                disText.setAttribute('id', 'distance-text');
                var cuisText = document.createElement('p');
                cuisText.innerHTML = 'Cuisine: ' + restaurants.get(i).cuisine;
                cuisText.setAttribute('id', 'restaurant-cuisine');
                div.appendChild(nameText);
                div.appendChild(ratText);
                div.appendChild(disText);
                div.appendChild(cuisText);
                div.setAttribute("class", "results");
                resultsForm.appendChild(div);
            }
            initialized = true;
        }
        else {
            var divs = resultsForm.querySelectorAll('div');
            for (var k = 0; k < 100; k++) {
                nameText = divs[k].querySelector('#restaurant-name');
                nameText.innerHTML = 'Name: ' + restaurants.get(k).name;
                ratText = divs[k].querySelector('#rating-text')
                ratText.innerHTML = 'Rating: ' + restaurants.get(k).rating;
                disText = divs[k].querySelector('#distance-text')
                disText.innerHTML = 'Distance: ' + restaurants.get(k).distance.toFixed(2);
                cuisText = divs[k].querySelector('#restaurant-cuisine')
                cuisText.innerHTML = 'Cuisine: ' + restaurants.get(k).cuisine;
                if (divs[k].style.display === 'none') {
                    divs[k].style.display = 'block';
                }
            }
        }
    }

    /* search functionality */
    var searchButton = document.querySelector('#s-cover');

    searchButton.addEventListener("click", searchItems);

    function searchItems(elmnt) {
        var searchString = document.getElementById('search').value;

        if (!initialized) {
            alert('please apply your preferences before searching');
            return;
        }

        var index = 0;
        var resultsForm = document.getElementById('results');
        var divs = resultsForm.querySelectorAll('div');

        for (var j = 0; j<restaurants.size(); j++) {
            if (index === 100) {
                break;
            }
            if ((restaurants.get(j).name.toLocaleLowerCase()).includes(searchString.toLowerCase())) {
                nameText = divs[index].querySelector('#restaurant-name');
                nameText.innerHTML = 'Name: ' + restaurants.get(j).name;
                ratText = divs[index].querySelector('#rating-text')
                ratText.innerHTML = 'Rating: ' + restaurants.get(j).rating;
                disText = divs[index].querySelector('#distance-text')
                disText.innerHTML = 'Distance: ' + restaurants.get(j).distance.toFixed(2);
                cuisText = divs[index].querySelector('#restaurant-cuisine')
                cuisText.innerHTML = 'Cuisine: ' + restaurants.get(j).cuisine;
                if (divs[index].style.display === 'none') {
                    divs[index].style.display = 'block';
                }
                index++;
            }
        }
        while (index < 100) {
            divs[index].style.display = 'none';
            index++;
        }
    }

}