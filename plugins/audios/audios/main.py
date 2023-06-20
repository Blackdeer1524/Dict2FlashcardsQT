"""
Credits:
    https://github.com/Rascalov/Anki-Simple-Forvo-Audio
"""


import re
from typing import Any, Literal, TypedDict, Union

import requests.utils

from .consts import _PLUGIN_LOCATION, _PLUGIN_NAME
from .page_processing import get_audio_link, get_forvo_page

CACHED_RESULT = {}

REMOVE_SPACES_PATTERN = re.compile(r"\s+", re.MULTILINE)


def remove_spaces(string: str) -> str:
    return re.sub(REMOVE_SPACES_PATTERN, " ", string.strip())


def get(word: str):
    global CACHED_RESULT

    word_with_lang_code = "{} {}".format(word, "en")

    if (audioListLis := CACHED_RESULT.get(word_with_lang_code)) is None:
        wordEncoded = requests.utils.requote_uri(word)
        forvoPage, error_message = get_forvo_page(
            "https://forvo.com/word/" + wordEncoded
        )
        if error_message:
            return [], error_message
        speachSections = forvoPage.select("div#language-container-" + "en")
        if not len(speachSections):
            return (
                [],
                f"[{_PLUGIN_NAME}] Word not found (Language Container does not exist!)",
            )
        speachSections = forvoPage.select_one("div#language-container-" + "en")
        audioListUl = speachSections.select_one("ul")
        if audioListUl is None or not len(
            audioListUl.findChildren(recursive=False)
        ):
            return (
                [],
                f"[{_PLUGIN_NAME}] Word not found (Language Container exists, but audio not found)",
            )
        # if config["language_code"] == "en":
        audioListLis = forvoPage.select("li[class*=en_]")
        # else:
        #     audioListLis = audioListUl.find_all("li")

        if audioListLis:
            CACHED_RESULT[word_with_lang_code] = audioListLis

    audio_batch: list[tuple[str, str]] = []
    batch_size = yield
    for li in audioListLis:
        if (r := li.find("div")) is not None and (
            onclick := r.get("onclick")
        ) is not None:
            audio_link = get_audio_link(onclick)
            by_whom_data = li.find("span", {"class": "info"})
            by_whom_data = (
                remove_spaces(by_whom_data.text)
                if by_whom_data is not None
                else ""
            )
            from_data = li.find("span", {"class": "from"})
            from_data = (
                remove_spaces(from_data.text) if from_data is not None else ""
            )
            additional_info = (
                (f"{by_whom_data}\n{from_data}")
                if from_data is not None
                else ""
            )
            audio_batch.append((audio_link, additional_info))
            if len(audio_batch) == batch_size:
                batch_size = yield {"web": audio_batch, "local": []}, ""
                audio_batch = []
    return audio_batch, ""


def load():
    return


def unload():
    return


ConfigKeys = Literal["audio region", "timeout"]
AUDIO_REGION = "audio region"
TIMEOUT = "timeout"


class ConfigFieldInfo(TypedDict):
    docs: str
    type: str


ConfigDescription = dict[
    ConfigKeys, Union[ConfigFieldInfo, "ConfigDescription"]
]


def get_config_description() -> ConfigDescription:
    return {
        AUDIO_REGION: {
            "docs": "Current audio region",
            "type": "string",
        },
        TIMEOUT: {"docs": "Request timeout", "type": "number"},
    }


def get_default_config() -> dict[ConfigKeys, Any]:
    return {AUDIO_REGION: "us", TIMEOUT: 3}


class ErrorSummary(TypedDict):
    error_type: Literal[
        "invalid_type", "invalid_value", "empty", "unknown_field"
    ]
    description: str


SetConfigError = dict[str, Union[ErrorSummary, "SetConfigError"]]


def validate_config(new_config: dict) -> SetConfigError:
    res: SetConfigError = {}

    if (new_audio_region := new_config.get(AUDIO_REGION)) is not None:
        if type(new_audio_region) != str:
            res[AUDIO_REGION] = {
                "error_type": "invalid_type",
                "description": f"`{AUDIO_REGION}` is expected to be a string",
            }
        elif new_audio_region not in ["uk", "us"]:
            res[AUDIO_REGION] = {
                "error_type": "invalid_value",
                "description": f'`{AUDIO_REGION}` is expected to be one of: ["uk", "us"]',
            }
    else:
        res[AUDIO_REGION] = {
            "error_type": "empty",
            "description": f"`{AUDIO_REGION}` was left empty",
        }

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

    conf_keys = [AUDIO_REGION, TIMEOUT]
    for key in new_config:
        if key in conf_keys:
            continue
        res[key] = {
            "error_type": "unknown_field",
            "description": f"Unknown field: `{key}`",
        }

    return res
