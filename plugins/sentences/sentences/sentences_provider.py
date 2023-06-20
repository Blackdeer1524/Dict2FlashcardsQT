import os
from typing import Any, Literal, TypedDict, Union

import bs4
import requests

FILE_PATH = os.path.split(os.path.dirname(__file__))[-1]


def get(word: str):
    try:
        page = requests.get(
            f"https://searchsentences.com/words/{word}-in-a-sentence",
        )
        page.raise_for_status()
    except requests.RequestException as e:
        return [], f"{FILE_PATH} couldn't get a web page: {e}"

    soup = bs4.BeautifulSoup(page.content, "html.parser")
    src = soup.find_all("li", {"class": "sentence-row"})
    sentences = []
    for sentence_block in src:
        if (sentence := sentence_block.find("span")) is None:
            continue
        text = sentence.get_text()
        if text:
            sentences.append(text)

    sentences.sort(key=len)
    i = 0
    size = yield
    while i < len(sentences):
        size = yield sentences[i : i + size], ""
        i += size

    return None 


def load():
    return


def unload():
    return


ConfigKeys = Literal["timeout"]
TIMEOUT = "timeout"


class ConfigFieldInfo(TypedDict):
    docs: str
    type: str


ConfigDescription = dict[
    ConfigKeys, Union[ConfigFieldInfo, "ConfigDescription"]
]


def get_config_description() -> ConfigDescription:
    return {
        TIMEOUT: {"docs": "Request timeout", "type": "number"},
    }


def get_default_config() -> dict[ConfigKeys, Any]:
    return {TIMEOUT: 3}


class ErrorSummary(TypedDict):
    error_type: Literal[
        "invalid_type", "invalid_value", "empty", "unknown_field"
    ]
    description: str


SetConfigError = dict[str, Union[ErrorSummary, "SetConfigError"]]


def validate_config(new_config: dict) -> SetConfigError:
    res: SetConfigError = {}

    if (new_timeout := new_config.get(TIMEOUT)) is not None:
        if type(new_timeout) not in [float, int]:
            res[TIMEOUT] = {
                "error_type": "invalid_type",
                "description": f"`{TIMEOUT}` is expected to be a number. Got `{new_timeout}`",
            }
        elif new_timeout < 0:
            res[TIMEOUT] = {
                "error_type": "invalid_value",
                "description": f"`{TIMEOUT}` is expected to be non negative",
            }

    else:
        res[TIMEOUT] = {
            "error_type": "empty",
            "description": "`timeout` was left empty",
        }

    conf_keys = [TIMEOUT]
    for key in new_config:
        if key in conf_keys:
            continue
        res[key] = {
            "error_type": "unknown_field",
            "description": f"Unknown field: `{key}`",
        }

    return res
