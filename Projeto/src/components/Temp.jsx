import React, { useState, useEffect } from "react";

const api = {
    key: "69f27d6f651417a4db87a0d2b498b441",
    base: "https://api.openweathermap.org/data/2.5",
};

const fetchWeatherData = async () => {
    try {
        const response = await fetch(`${api.base}/weather?q=Braga&appid=${api.key}`);
        const data = await response.json();
        return data;
    } catch (error) {
        console.error('Error fetching weather data:', error);
        return null;
    }
};

const kelvinToCelsius = (tempInKelvin) => {
    return tempInKelvin - 273.15;
};

const getWeatherTemperature = async () => {
    const weatherData = await fetchWeatherData();
    if (weatherData) {
        return kelvinToCelsius(weatherData.main.temp);
    } else {
        return null;
    }
};

function Temp(props) {
    const [temperature, setTemperature] = useState(null);

    useEffect(() => {
        const loadTemperature = async () => {
            const temp = await getWeatherTemperature();
            setTemperature(temp);
        };
        loadTemperature();
        const intervalId = setInterval(loadTemperature, 60 * 1000);
        return () => clearInterval(intervalId);
    }, []);

    const getTemperatureColor = (temp) => {
        if (temp >= 25) {
            return "#FF5733"; // Reddish orange
        } else if (temp >= 20) {
            return "#FF884B"; // Light orange
        } else if (temp >= 15) {
            return "#FFB366"; // Peach
        } else if (temp >= 10) {
            return "#FFE0A3"; // Light peach
        } else if (temp >= 5) {
            return "#EBF2FA"; // Light blue
        } else if (temp >= 0) {
            return "#CCE0FF"; // Light sky blue
        } else if (temp >= -5) {
            return "#99C2FF"; // Sky blue
        } else if (temp >= -10) {
            return "#66A3FF"; // Light blue
        } else if (temp >= -15) {
            return "#3385FF"; // Blue
        } else if (temp >= -20) {
            return "#0057FF"; // Deep blue
        } else if (temp >= -25) {
            return "#003D99"; // Dark blue
        } else {
            return "#001A66"; // Very dark blue
        }
    };


    return (<span className = {props.name} style={{color: "white"}}>
        {Math.round(temperature)} °C <span style={{color: getTemperatureColor(temperature)}}>BRAGA</span>
        </span>);
}

export default Temp;
