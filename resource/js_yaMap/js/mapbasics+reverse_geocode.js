var myMap;
var globCoords
// Дождёмся загрузки API и готовности DOM.
ymaps.ready(init);

function init () {
    // Создание экземпляра карты и его привязка к контейнеру с
    // заданным id ("map").
    myMap = new ymaps.Map('map', {
        // При инициализации карты обязательно нужно указать
        // её центр и коэффициент масштабирования.
        center: [57.264969,44.532513], // City
        zoom: 12
    }, {
        searchControlProvider: 'yandex#search'
    },);
    globCoords = myMap.getCenter(); //получу начальные координаты центра карты
    foo.getCoordsFromScript(globCoords);
    var myPlacemark;
    // Слушаем клик на карте.
    myMap.events.add('click', function (e) {
        var coords = e.get('coords');
        foo.getCoordsFromScript(coords);
        // Если метка уже создана – просто передвигаем ее.
        if (myPlacemark) {
            myPlacemark.geometry.setCoordinates(coords);
        }
        // Если нет – создаем.
        else {
            myPlacemark = createPlacemark(coords);
            myMap.geoObjects.add(myPlacemark);
            // Слушаем событие окончания перетаскивания на метке.
            myPlacemark.events.add('dragend', function () {
                getAddress(myPlacemark.geometry.getCoordinates());
            });
        }
        getAddress(coords);
    });

    // Создание метки.
    function createPlacemark(coords) {
        return new ymaps.Placemark(coords, {
            iconCaption: 'поиск...'
        }, {
            preset: 'islands#violetDotIconWithCaption',
            draggable: true
        });
    }

    // Определяем адрес по координатам (обратное геокодирование).
    function getAddress(coords) {
        myPlacemark.properties.set('iconCaption', 'поиск...');
        ymaps.geocode(coords).then(function (res) {
            var firstGeoObject = res.geoObjects.get(0);

            myPlacemark.properties
                .set({
                    // Формируем строку с данными об объекте.
                    iconCaption: [
                        // Название населенного пункта или вышестоящее административно-территориальное образование.
                        firstGeoObject.getLocalities().length ? firstGeoObject.getLocalities() : firstGeoObject.getAdministrativeAreas(),
                        // Получаем путь до топонима, если метод вернул null, запрашиваем наименование здания.
                        firstGeoObject.getThoroughfare() || firstGeoObject.getPremise()
                    ].filter(Boolean).join(', '),
                    // В качестве контента балуна задаем строку с адресом объекта.
                    balloonContent: firstGeoObject.getAddressLine() + '\r\n' + coords
                });
        });
    }

//    document.getElementById('destroyButton').onclick = function () {
//        // Для уничтожения используется метод destroy.
//        myMap.destroy();
//    };

}

function setCenter () {
    myMap.setCenter([67.02097139978888, 34.12532682715678]);
    //foo.jscallme(globCoords);
    //foo.getCoordsFromScript(globCoords);
}

function setBounds () {
    // Bounds - границы видимой области карты.
    // Задаются в географических координатах самой юго-восточной и самой северо-западной точек видимой области.
    myMap.setBounds([[37, 38], [39, 40]]);
}

function setTypeAndPan () {
    // Меняем тип карты на "Гибрид".
    myMap.setType('yandex#hybrid');
    // Плавное перемещение центра карты в точку с новыми координатами.
    myMap.panTo([62.915, 34.461], {
            // Задержка между перемещениями.
            delay: 500
        });
}
