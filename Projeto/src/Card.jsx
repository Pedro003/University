import Logo from './assets/Logo.png'

function Card(){
    return(
        <div className="card">
            <img className="card-image" src={Logo} alt="profile picture"></img>
            <h2 className='card-title'>Pedro</h2>
            <p className='card-text'>Eu estudo Ciências da Computação</p>
        </div>
    );
}

export default Card