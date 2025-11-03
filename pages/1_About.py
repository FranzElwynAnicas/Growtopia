import streamlit as st

st.set_page_config(page_title="About | Growtopia", layout="wide")

# style
page_style = """
<style>
[data-testid="stAppViewContainer"] {
    background-color: #e9f7ec;
    background-size: cover;
    background-position: center;
}

[data-testid="stSidebar"] {
    background-color: #d8f3dc;
    color: #1b4332;
}
[data-testid="stSidebar"] * {
    color: #1b4332 !important;
    font-weight: 600;
}

[data-testid="stHeader"] {
    background: rgba(0,0,0,0);
}

.main-container {
    background: rgba(255, 255, 255, 0.94);
    padding: 3rem;
    border-radius: 20px;
    box-shadow: 0 4px 20px rgba(0,0,0,0.1);
    max-width: 900px;
    margin: 4rem auto;
    text-align: center;
    color: #1b4332;
}

h1 {
    color: #1b4332 !important;
    font-weight: 800;
}
h2, h3, p {
    color: #2d6a4f !important;
}

img {
    border-radius: 15px;
    max-width: 85%;
    height: auto;
    margin-top: 1.5rem;
    margin-bottom: 1.5rem;
}
</style>
"""
st.markdown(page_style, unsafe_allow_html=True)

# wrapper
st.markdown('<div class="main-container">', unsafe_allow_html=True)

st.title("🌱 About Growtopia")

st.write("""
Growtopia is an **IoT-based smart watering system** designed to help Filipinos embrace 
sustainable gardening. It uses real-time soil moisture to automate irrigation,
ensuring your plants get just the right amount of water.

Our goal is to make smart agriculture **accessible**, **efficient**, and **eco-friendly** 
for every home, garden, and small farm.
""")

# history
st.write("""
I’ve always loved planting, but I often forgot to water my plants, or sometimes overwatered them, 
especially when I was away from home or overslept.  
That small struggle made me realize how many people might be facing the same issue.

From that thought, **Growtopia** was born, a smart watering system that takes the guesswork out of plant care.  
It started as a simple idea to help me care for my own plants, and grew into a project 
that aims to help others save water, time, and effort while promoting sustainable living.
""")

# image
st.markdown(
    """
    <div style='text-align: center;'>
        <img src='https://i.imgur.com/W89jxr0_d.webp?maxwidth=760&fidelity=grand' 
             width='580' 
             style='border-radius:15px;'>
        <p style='color:#2d6a4f;'><i>Growtopia Smart Irrigation Overview</i></p>
    </div>
    """,
    unsafe_allow_html=True
)



st.caption("© 2025 Growtopia | Making gardening an enjoyable and sustainable activity for everyone.")


st.markdown('</div>', unsafe_allow_html=True)
