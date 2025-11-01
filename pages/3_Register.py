import streamlit as st
import gspread
from oauth2client.service_account import ServiceAccountCredentials
from datetime import datetime

# --- Page Setup ---
st.set_page_config(page_title="GrowSense | Register", layout="wide")

# --- Styling ---
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
    max-width: 700px;
    margin: 4rem auto;
    text-align: center;
    color: #1b4332;
}

h1, h2, h3, p, label {
    color: #2d6a4f !important;
}
.stTextInput label, .stTextArea label {
    font-weight: 600;
}

.stButton>button {
    background-color: #2d6a4f;
    color: white;
    font-weight: 600;
}

/* Input box styling */
div.stTextInput>div>div>input, 
div.stTextArea>div>div>textarea {
    background-color: #f0f9f4 !important;  /* soft pastel white */
    color: #1b4332 !important;            /* dark text for readability */
    border: 2px solid #a7c7a9 !important; /* light green border */
    border-radius: 10px !important;
}

/* Placeholder text color */
div.stTextInput>div>div>input::placeholder, 
div.stTextArea>div>div>textarea::placeholder {
    color: #52796f !important;
}
</style>
"""
st.markdown(page_style, unsafe_allow_html=True)

# --- Google Sheets Setup using Secrets ---
scope = ["https://spreadsheets.google.com/feeds", "https://www.googleapis.com/auth/drive"]
creds_dict = st.secrets["google_service_account"]  # <-- direct dict from secrets
creds = ServiceAccountCredentials.from_json_keyfile_dict(creds_dict, scope)
client = gspread.authorize(creds)
sheet = client.open("Growtopia signups").sheet1  # Make sure the sheet name matches exactly

# --- Main Container ---
st.markdown('<div class="main-container">', unsafe_allow_html=True)

st.title("📩 Contact / Register")
st.write("Join our early access list and get updates about GrowSense!")

with st.form("signup_form"):
    name = st.text_input("Full Name")
    email = st.text_input("Email Address")
    message = st.text_area("Message (optional)")
    submit = st.form_submit_button("Sign Up")

if submit:
    if name and email:
        timestamp = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
        sheet.append_row([timestamp, name, email, message])
        st.success("✅ Thank you! Your information has been recorded.")
    else:
        st.warning("Please fill in your name and email.")

st.caption("© 2025 GrowSense | Making gardening an enjoyable and sustainable activity for everyone.")
st.markdown('</div>', unsafe_allow_html=True)
