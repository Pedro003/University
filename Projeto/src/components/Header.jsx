import Button_Route from "./Button_Route";
import Switch from "./Switch";
import Data from "./Data";
import Temp from "./Temp";

function Header(){

    return (
        <>
        <div className="header-line">
            <Button_Route page="/home" class_button="home"/>
            <Data/>
            <Temp name="temperatura"/>
            <span className="menu-links">
                <a href="/about">ABOUT US</a>
                <a href="/log-in">LOG IN</a>
            </span>
        </div>
        <hr className="upper-line"/>
        </>
    );
}

export default Header