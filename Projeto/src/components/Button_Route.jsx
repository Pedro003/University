import React from 'react';
import { useNavigate } from 'react-router-dom';

function Button_Route (props) {
    const navigate = useNavigate();

    const handleClick = () => {
        // Navigate to the desired page
        navigate(props.page);
    };

    return (
        <button onClick={handleClick} className={props.class_button}>
                <p className={props.class_button_name}>{props.name}</p>
        </button>
    );
}

export default Button_Route;